#---------------------------------
# INITIALISATION DES VARIABLES 
#---------------------------------

# Indiquer le compilateur
CXX= gcc

# Les chemins ou se trouvent les fichiers a inclure
INCLUDE= -I/usr/include

# Options de compilation.
CXXFLAGS=  $(INCLUDES_DIR) -c -Wno-deprecated

# Les chemins ou se trouvent les librairies
LIBRARY_PATH= -L/usr/lib

# Options pour le linker.
LDFLAGS= $(LIBRARY_PATH) -o

# Les librairies avec lesquelle on va effectueller l'edition de liens
LIBS=  

# Les fichiers sources de l'application
FILES= main.c sem.c interface.c client.c serveur.c voiture.c carrefour.c

#-----------
# LES CIBLES
#-----------
all:  $(FILES:.c=.o)
	$(CXX) $(LDFLAGS) project $(FILES:.c=.o) $(LIBS)

.PHONY : clean
clean:
	/bin/rm $(FILES:.c=.o) project

#-----------------------------------------------------------------------------
# LES REGLES DE DEPENDANCE. Certaines sont implicites mais je recommande d'en 
# mettre une par fichier source. 
#-----------------------------------------------------------------------------

#---------------------------------
# REGLES DE COMPILATION IMPLICITES
#---------------------------------
%.o : %.c ; $(CXX) $(CXXFLAGS) $*.c
