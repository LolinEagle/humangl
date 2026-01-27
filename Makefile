NAME		=	humangl
SRC			=	main.cpp\
				Controller.cpp\
				MainClass.cpp\
				PointLight.cpp\
				SimpleRender.cpp\
				vem.cpp\
				VulkanEngine/VeBuffer.cpp\
				VulkanEngine/VeCamera.cpp\
				VulkanEngine/VeDescriptors.cpp\
				VulkanEngine/VeDevice.cpp\
				VulkanEngine/VeGameObject.cpp\
				VulkanEngine/VeModel.cpp\
				VulkanEngine/VePipeline.cpp\
				VulkanEngine/VeRenderer.cpp\
				VulkanEngine/VeSwapChain.cpp\
				VulkanEngine/VeWindow.cpp

# Commands
CPP			=	c++
RM			=	rm -rf
GLSLC		=	./shader/glslc

# Flags
CPP_FLAGS	=	-std=c++20 -MMD
LIB_FLAGS	=	-lglfw -lvulkan -ldl -lpthread -lpng
X11_FLAGS	=	-lX11 -lXxf86vm -lXrandr -lXi

# Path
INC			=	-Iinc -Iinc/VulkanEngine
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/
OBJ_PATH_VE	=	./obj/VulkanEngine/

# Objects
OBJ_DIRS	=	${OBJ_PATH}
OBJ			=	${addprefix ${OBJ_PATH},${SRC:.cpp=.o}}
DEP			=	${addprefix ${OBJ_PATH},${SRC:.cpp=.d}}

SHADER_PATH	=	./shader/
SPIRV_PATH	=	./spirv/

SHADER_SRC	=	simpleShader.vert \
				simpleShader.frag \
				pointLight.vert \
				pointLight.frag

SPIRV_OBJ	=	${addprefix ${SPIRV_PATH},${addsuffix .spv,${SHADER_SRC}}}

all:${NAME}

clean:
	${RM} ${OBJ_PATH} .vscode

shaderclean:
	${RM} ${SPIRV_PATH}

fclean:clean shaderclean
	${RM} ${NAME}

re:fclean
	make -j all

run:re
	clear
	./${NAME} 1

shader:${SPIRV_OBJ}

${SPIRV_PATH}%.spv:${SHADER_PATH}% ${SPIRV_PATH}
	${GLSLC} $< -o $@

${OBJ_PATH}%.o:${SRC_PATH}%.cpp
	${CPP} ${CPP_FLAGS} ${INC} -c $< -o $@

${OBJ_DIRS}:
	mkdir ${OBJ_DIRS} ${OBJ_PATH_VE}
${SPIRV_PATH}:
	mkdir ${SPIRV_PATH}

${NAME}:${OBJ_DIRS} ${OBJ} ${SPIRV_OBJ}
	${CPP} ${OBJ} ${LIB_FLAGS} ${X11_FLAGS} -o $@

.PHONY:all clean shaderclean fclean re run shader

-include ${DEP}
