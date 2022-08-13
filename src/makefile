install: glad.o app.o
	g++ -o output $? -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	rm *.o

glad.o: glad.c
	g++ -c $?
app.o: app.cpp
	g++ -c $?

remove: app.o glad.o app.o
	rm *.o output

clean:
	find . -type f | xargs touch
	rm -rf $(OBJS)
