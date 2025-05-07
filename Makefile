CXX = c++
CXXFLAGS = -std=c++11 -O3 -w -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -lomp -L/opt/homebrew/opt/libomp/lib

SOURCES = bounding_box.cpp main.cpp sphere.cpp scene.cpp triangle_mesh.cpp vector.cpp 
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

spheres: $(TARGET)
	./$(TARGET) spheres

cat: $(TARGET)
	./$(TARGET) cat

combined: $(TARGET)
	./$(TARGET) combined

clean:
	rm -f $(TARGET)
