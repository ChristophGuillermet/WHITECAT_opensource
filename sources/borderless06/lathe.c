#include <allegro.h>
#include <winalleg.h>
#include <alleggl.h>

#include <GL/glu.h>
#include "monitor.h"
#include "window.h"
#include <math.h>

#define buildVector(a,b,c) \
	(a)[0] = (c)[0] - (b)[0]; \
	(a)[1] = (c)[1] - (b)[1]; \
	(a)[2] = (c)[2] - (b)[2];

#define copyPoint(a,b) \
	(a)[0] = (b)[0]; \
	(a)[1] = (b)[1]; \
	(a)[2] = (b)[2];


#define POINT_COUNT     11

typedef struct PLACE
{
    GLfloat x, y;
} PLACE;


void draw_square(GLfloat x, GLfloat y, GLfloat side)
{
    GLfloat s = side / 2;
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - s, y - s);
        glVertex2f(x + s, y - s);
        glVertex2f(x + s, y + s);
        glVertex2f(x - s, y + s);
    glEnd();
}


volatile int rotation_counter = 0;
void rotation_counter_handler()
{
    rotation_counter++;
}
END_OF_FUNCTION(rotation_counter_handler)


int DESKTOP_W = 0;
int DESKTOP_H = 0;

int main()
{
    /* Setup Allegro/AllegroGL */

	if (allegro_init())
		return 1;

	if (install_allegro_gl())
		return 1;

    if (install_keyboard() < 0)
    {
        allegro_message("Unable to install keyboard\n");
        return 1;
    }

    if (install_mouse() == -1)
    {
        allegro_message("Unable to install mouse\n");
        return 1;
    }

    if (install_timer() < 0)
    {
        allegro_message("Unable to install timers\n");
        return 1;
    }

    /* lock timer */
    LOCK_VARIABLE(rotation_counter);
    LOCK_FUNCTION(rotation_counter_handler);


    /* set desktop resolution */
    DESKTOP_W = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    DESKTOP_H = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    /* get monitor resolution/count */
    int monitor_count;
    MONITOR *monitors = get_monitors(&monitor_count);


    /* generate point data */
    PLACE places[POINT_COUNT];
    int c;
    for (c = 1; c < POINT_COUNT - 1; c++)
    {
        places[c].x = sin((M_PI * (GLfloat)c) / 10.0f) * 200.0f;
        places[c].y = cos((M_PI * (GLfloat)c) / 10.0f) * 200.0f;
    }
    places[0].x = 0.01;
    places[0].y = 200.0f;
    places[POINT_COUNT - 1].x = 0.01;
    places[POINT_COUNT - 1].y = -200.0f;


    /* setup display */
    allegro_gl_set(AGL_Z_DEPTH, 8);
	allegro_gl_set(AGL_COLOR_DEPTH, 16);
	allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH | AGL_COLOR_DEPTH);
    glDepthFunc(GL_LEQUAL);

	if (set_gfx_mode(GFX_OPENGL_WINDOWED_BORDERLESS, DESKTOP_W, DESKTOP_H, 0, 0)) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Unable to set graphic mode\n%s\n", allegro_error);
		return 1;
	}

    /* move window so it covers the desktop */
    position_window(0, 0);


    /* fake information to use if only 1 monitor */
    MONITOR fake = {0, 512, 512, 512};

    /* setup lighting model */
    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat ambient[] = { 0.1f, 0.1f, 0.1f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);


    int selected = -1; /* the point currently being moved */
    GLfloat rotation[3] = {0, 0, 0}; /* the rotation of the mesh */

    install_int(rotation_counter_handler, 20); /* install the rotation handler */

    /* enter main program loop */
    while(!key[KEY_ESC])
    {
        while (rotation_counter > 0)
        {
            /* rotate the mesh */
            rotation[0] += M_PI / 24.0f;
            rotation[1] += M_PI / 16.0f;
            rotation[2] += M_PI /  8.0f;
            rotation_counter--;
        }


        /* clear the buffers */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* process monitor 0 */
        MONITOR *m = &monitors[0];

        /* adjust mouse so its relative to the monitor */
        int mx = (mouse_x - m->x) - (m->w / 2);
        int my = (mouse_y - m->y) - (m->h / 2);

        /* if the left mouse is pushed, find a close point */
        if (mouse_b & 1)
        {
            if (selected == -1)
            {
                GLfloat distance = 10;
                for (c = 0; c < POINT_COUNT; c++)
                {
                    GLfloat dx = mx - places[c].x;
                    GLfloat dy = my - places[c].y;
                    GLfloat d = sqrt(dx * dx + dy * dy);
                    if (d < distance)
                    {
                        distance = d;
                        selected = c;
                    }
                }
            }
        }
        else
            selected = -1;

        /* move selected point */
        if (selected >= 0)
        {
            places[selected].x = mx;
            places[selected].y = my;
        }

        /* center the viewport on monitor */
        glViewport(m->x, DESKTOP_H - m->h - m->y, m->w, m->h);

        /* setup viewport projection */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-m->w / 2, m->w / 2, m->h / 2, -m->h / 2);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* draw points */
        glColor3ub(0, 255, 0);
        glBegin(GL_LINE_STRIP);
        for (c = 0; c < POINT_COUNT; c++)
        {
            glVertex2f(places[c].x, places[c].y);
        }
        glEnd();

        glColor3ub(255, 255, 255);
        for (c = 0; c < POINT_COUNT; c++)
        {
            draw_square(places[c].x, places[c].y, 10);
        }

        /* draw vertical line */
        glBegin(GL_LINE_STRIP);
        glVertex2f(0.0f, -m->h);
        glVertex2f(0.0f, m->h);
        glEnd();


        /* draw the mouse */
        glColor3ub(255, 255, 255);
        draw_square(mx, my, 20);



        /* process viewport 1 */

        /* select second monitor */
        if (monitor_count > 1)
        {
            /* if 2nd monitor exists use it */
            m = &monitors[1];
        }
        else
        {
            /* use fake monitor */
            m = &fake;
        }

        /* adjust mouse so its relative to the monitor*/
        mx = (mouse_x - m->x) - (m->w / 2);
        my = (mouse_y - m->y) - (m->h / 2);

        /* center the viewport on the monitor*/
        glViewport(m->x, DESKTOP_H - m->h - m->y, m->w, m->h);

        /* setup viewport projection */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)m->w / (float)m->h, 0.1f, 2000.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* turn on lighting and depth testing */
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

        /* move mesh so its visible */
        glTranslatef(0.0f, 0.0f, -1000.0f);
        /* rotate mesh */
        glRotatef(rotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(rotation[1], 0.0f, 1.0f, 0.0f);
        glRotatef(rotation[2], 0.0f, 0.0f, 1.0f);

        GLfloat p1[3] = {0, 0, 0};
        GLfloat p2[3] = {0, 0, 0};
        GLfloat p3[3] = {0, 0, 0};
        GLfloat p4[3] = {0, 0, 0};
        GLfloat vec1[3];
        GLfloat vec2[3];
        GLfloat normal[3];


        /* draw mesh to screen */
        glColor3ub(0, 255, 0);
        for (c = 0; c < (POINT_COUNT - 1); c++)
        {

            GLfloat a1 = 0;
            GLfloat a2 = M_PI / 16.0f;
            GLfloat d1 = places[c].x;
            GLfloat d2 = places[c + 1].x;

            p1[0] = sin(a1) * d1;  p1[1] = places[c].y;     p1[2] = cos(a1) * d1;
            p2[0] = sin(a2) * d1;  p2[1] = places[c].y;     p2[2] = cos(a2) * d1;
            p3[0] = sin(a2) * d2;  p3[1] = places[c + 1].y; p3[2] = cos(a2) * d2;
            p4[0] = sin(a1) * d2;  p4[1] = places[c + 1].y; p4[2] = cos(a1) * d2;

            buildVector(vec1, p1, p2);
            buildVector(vec2, p1, p4);
            cross_product_f(vec2[0], vec2[1], vec2[2], vec1[0], vec1[1], vec1[2], &normal[0], &normal[1], &normal[2]);
            normalize_vector_f(&normal[0], &normal[1], &normal[2]);


            glBegin(GL_QUAD_STRIP);
            glNormal3fv(normal);
            glVertex3fv(p1);
            glVertex3fv(p4);

            int s = 0;
            for (s = 1; s < 32; s++)
            {
                a2 = (M_PI * (GLfloat)(s + 1)) / 16.0f;
                d1 = places[c].x;
                d2 = places[c + 1].x;

                copyPoint(p1, p2);
                copyPoint(p4, p3);
                p2[0] = sin(a2) * d1;  p2[1] = places[c].y;     p2[2] = cos(a2) * d1;
                p3[0] = sin(a2) * d2;  p3[1] = places[c + 1].y; p3[2] = cos(a2) * d2;

                buildVector(vec1, p1, p2);
                buildVector(vec2, p1, p4);
                cross_product_f(vec2[0], vec2[1], vec2[2], vec1[0], vec1[1], vec1[2], &normal[0], &normal[1], &normal[2]);
                normalize_vector_f(&normal[0], &normal[1], &normal[2]);

                glNormal3fv(normal);
                glVertex3fv(p2);
                glVertex3fv(p3);
            }
            glEnd();
        }

        /* turn off lighting and depth testing */
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        /* if not using the fake monitor */
        if (m != &fake)
        {

            /* setup viewport projection */
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(-m->w / 2, m->w / 2, m->h / 2, -m->h / 2);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            /* draw the mouse */
            glColor3ub(255, 255, 255);
            draw_square(mx, my, 20);
        }

        /* flip the contents to the screen */
        allegro_gl_flip();
    }


    free(monitors);

    return 0;
}
END_OF_MAIN()
