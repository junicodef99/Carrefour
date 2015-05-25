#---------------------------------
# INITIALISATION DES VARIABLES 
#---------------------------------

# Indiquer le compilateur
CXX= gcc

# Les chemins ou se trouvent les fichiers a inclure
INCLUDE= -I/

# Options de compilation.
CXXFLAGS=  $(INCLUDES_DIR) -c -Wno-deprecated

# Les chemins ou se trouvent les librairies
LIBRARY_PATH= -L/usr/lib

# Options pour le linker.
LDFLAGS= $(LIBRARY_PATH) -o

# Les librairies avec lesquelle on va effectueller l'edition de liens
LIBS=  

# Les fichiers sources de l'application
FILES= project.c 

#-----------
# LES CIBLES
#-----------
executable :  $(FILES:.cc=.o)
	$(CXX) $(LDFLAGS) executable $(FILES:.cc=.o) $(LIBS)

.PHONY : clean
clean:
	/bin/rm $(FILES:.cc=.o) executable

#-----------------------------------------------------------------------------
# LES REGLES DE DEPENDANCE. Certaines sont implicites mais je recommande d'en 
# mettre une par fichier source. 
#-----------------------------------------------------------------------------
project.o:project.c structs.h fonctions.h

#---------------------------------
# REGLES DE COMPILATION IMPLICITES
#---------------------------------
%.o : %.cc ; $(CXX) $(CXXFLAGS) $*.cc

