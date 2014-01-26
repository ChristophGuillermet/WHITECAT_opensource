
int do_logical_grand_master(int GMX, int GMY, int larg)
{

if(mouse_x>GMX && mouse_x<GMX+larg && mouse_y>=GMY-5 && mouse_y<=GMY+255)
{


if((window_focus_id==0 || window_focus_id==906 )&& mouse_button==1 && index_allow_grand_master==1)
{
set_mouse_range(GMX, GMY, GMX+larg, GMY+255);//pour pas deborder
//NIVEAU
niveauGMaster=((GMY+255)-mouse_y);  
if(niveauGMaster>255){niveauGMaster=255;}
if(niveauGMaster<0){niveauGMaster=0;}
midi_levels[615]=(niveauGMaster/2);
if(midi_send_out[615]==1){ index_send_midi_out[615]=1;}

//CONFIG MIDI
if(Midi_Faders_Affectation_Type!=0)//config midi
{
attribute_midi_solo_affectation(615,Midi_Faders_Affectation_Mode);

//midi report 
 switch(miditable[0][615])
 {
  case 0:
  sprintf(thetypinfo,"Note");
  break;
  case 1:
  sprintf(thetypinfo,"Key On");
  break;
  case 2:
  sprintf(thetypinfo,"Key Off");
  break;
  case 4:
  sprintf(thetypinfo,"Ctrl Change");
  break;
}
  sprintf(string_last_midi_id,"MIDI GRAND MASTER:Ch: %d Pitch: %d Type: %s", miditable[1][615],miditable[2][615],thetypinfo);  

mouse_released=1;
}                        
}       
}

//midi out on off
if(mouse_x>GMX+larg+30-10 && mouse_x<GMX+larg+30+10 && mouse_y>GMY+250-10 && mouse_y<GMY+250+10)
{
 if(mouse_button==1 && mouse_released==0)
 { 
  if(midi_send_out[615]==0){midi_send_out[615]=1; }        
  else if(midi_send_out[615]==1){midi_send_out[615]=0; }    
  mouse_released=1;           
  }                         
}

//raccrochage midi
raccrochage_midi_logical_vertical_dmx(GMX,GMY,615,larg,255);


return(0);   
}





int grand_master(int GMX, int GMY)
{
//DESSIN Master
Rect Gma( Vec2D(GMX,GMY), Vec2D(40,255) );//box du fader
Gma.SetRoundness(15);  
Gma.SetLineWidth(epaisseur_ligne_fader);
Rect GmaNiv( Vec2D(GMX,((GMY+255)-niveauGMaster)), Vec2D (40,niveauGMaster+2));//niveau fader
GmaNiv.SetRoundness(15);
switch(dmx_view)
{
case 0:
sprintf(string_niveauGMaster, "%d",(int)(((float)niveauGMaster)/2.55));
break;
case 1:
sprintf(string_niveauGMaster,"%d", niveauGMaster);
break;
}
GmaNiv.Draw(CouleurBlind);
Gma.DrawOutline(CouleurLigne);
neuro.Print(string_niveauGMaster,GMX, GMY-5); //niveau du fader
//bouton midi out

//midi out enclenché ou pas du Grand Master
Circle BMMidiOut( (GMX+70),(GMY+250), 10);//box du fader
BMMidiOut.SetLineWidth(epaisseur_ligne_fader);
if(midi_send_out[615]==1)
{BMMidiOut.Draw(CouleurBlind);}
BMMidiOut.DrawOutline(CouleurLigne);
Line (Vec2D( GMX+40,GMY+240),Vec2D( GMX+55,GMY+240)).Draw(CouleurLigne);
Line (Vec2D( GMX+55,GMY+240),Vec2D( GMX+65,GMY+245)).Draw(CouleurLigne);
//////////////////////////////////////

neuromoyen.Print("Grand Master",GMX-25, GMY+280);
if(mouse_x>GMX && mouse_x<GMX+40 && mouse_y>=GMY-5 && mouse_y<=GMY+255 && Midi_Faders_Affectation_Type!=0)
{
//affichage rouge config mùidi
Gma.DrawOutline(CouleurBlind);                        
}

//raccrochage midi
raccrochage_midi_visuel_vertical_dmx (GMX, GMY, 615,40,255);
return(0);   
}
