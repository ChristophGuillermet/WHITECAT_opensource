#demos/gamedemo/Makefile


	
EXE_SOURCES=Demo.cpp
EXE=gamedemo

ifeq "$(ENVIRONMENT)" "WINDOWS"
EXE :=$(EXE).exe
endif

ifeq "$(ENVIRONMENT)" "MSYS"
EXE :=$(EXE).exe
endif


all: $(EXE)

.PHONY: all clean

clean:
	@echo "Making clean..."
	$(RM) *.o
	$(RM) $(EXE)
	@echo "Demo is cleaned"

$(EXE): $(EXE_SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(EXE_SOURCES) ../../lib/$(LIB) $(LIBFLAGS) -o $@

	
	

	