NAME = scop
CC = g++ -Wall -Werror -Wextra
CFLAGS =  -std=c++17
OPENGL = -lglut -lGLU -lGL -lglfw -lX11

CPPSRCS =	main.cpp	\
			utils.cpp

CSRCS =	glad.c

MATH =	math.cpp	\
		mat4.cpp	\
		mat3.cpp	\
		mat2.cpp	\
		vec4.cpp	\
		vec3.cpp	\
		vec2.cpp

OBJECTS =	shader.cpp	\
			camera.cpp	\
			mesh.cpp 	\
			noise.cpp

csrcs =	$(addprefix srcs/, $(CSRCS))

cppsrcs =	$(addprefix srcs/, $(CPPSRCS))			\
			$(addprefix srcs/math/, $(MATH))		\
			$(addprefix srcs/objects/, $(OBJECTS))

srcs = $(csrcs)
srcs += $(cppsrcs)

objs =	$(csrcs:.c=.o)
objs += $(cppsrcs:.cpp=.o)

all: comp_obj

comp_obj: $(srcs)
	$(CC) $(CFLAGS) $(srcs) $(OPENGL) -o $(NAME)

clean:
	@rm -rf $(NAME)

fclean: clean
	@rm -f $(objs)

re: fclean all

.PHONY: all clean fclean re