#define STARSINSYSTEM 1000

struct face
{
	int vtx[3];
};

struct star
{
	float xPosition;
	float yPosition;
	float zPosition;
};

struct point
{
	float x;
	float y;
	float z;
};

struct model
{
	point *obj_points;
	face *obj_faces;
	int vertices;
	int faces;
};

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();
// Idle function

void Keyboard(unsigned char key, int x, int y);

void ReadFile(model*);															//Function for reading a model file

void DisplayModel(model);														// Function for displaying a model

void DrawAxes();																//Function for axes

void DrawStars();																//Function for stars drawing

void DrawSun();																		//Function for sun drawing

void DrawEarth();																	//Function for earth drawing

void DrawMoon();																	//Function for moon drawing

void DrawText(const char *str, float size);											//Function for text drawing