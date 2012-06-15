Attribute.o: Attribute.cpp Attribute.h gg.h
Circle.o: Circle.cpp Circle.h Object.h Shape.h Shader.h Attribute.h gg.h
Matrix.o: Matrix.cpp Matrix.h gg.h
Obj.o: Obj.cpp Obj.h Object.h Shape.h Shader.h Attribute.h gg.h
Object.o: Object.cpp Object.h Shape.h Shader.h Attribute.h gg.h
Shader.o: Shader.cpp Shader.h Attribute.h gg.h
Shape.o: Shape.cpp Shape.h Shader.h Attribute.h gg.h
SimpleShader.o: SimpleShader.cpp SimpleShader.h Shader.h Attribute.h gg.h \
 Matrix.h
Square.o: Square.cpp Square.h Shape.h Shader.h Attribute.h gg.h
Texture.o: Texture.cpp Texture.h Attribute.h gg.h
TileShader.o: TileShader.cpp TileShader.h SimpleShader.h Shader.h \
 Attribute.h gg.h Matrix.h
Trackball.o: Trackball.cpp Trackball.h gg.h
gg.o: gg.cpp gg.h
main.o: main.cpp gg.h Trackball.h Matrix.h Square.h Shape.h Shader.h \
 Attribute.h Obj.h Object.h SimpleShader.h TileShader.h
