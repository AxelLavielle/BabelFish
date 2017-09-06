SRCPATH		=	sources

SRC		=	$(SRCPATH)/Core.cpp		\
			$(SRCPATH)/LogParser.cpp	\
			$(SRCPATH)/Rotator.cpp		\
			$(SRCPATH)/AViewer.cpp		\
			$(SRCPATH)/TermViewer.cpp	\
			$(SRCPATH)/CursesViewer.cpp	\
			$(SRCPATH)/main.cpp

OBJ		=	${SRC:.cpp=.o}

CXXFLAGS	+=	-W -Wextra -Wall -Werror -ansi -pedantic
CXXFLAGS	+=	-pthread --std=c++11 -Iincludes -Ilibs/c++
LDFLAGS		+=	-lncurses

CXX		=	clang++

NAME		=	BabelFish

all		:	$(NAME)

$(NAME)		:	$(OBJ)
			$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

clean		:
			rm -f $(OBJ)

fclean		:	clean
			rm -f $(NAME)

re		:	fclean all
