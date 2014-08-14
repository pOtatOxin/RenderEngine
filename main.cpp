#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "texture.hpp"
#include "shader.h"
#include "objloader.hpp"
#define VIEWING_DISTANCE_MIN  3.0
#define TEXTURE_ID_CUBE 1
enum {
  MENU_LIGHTING = 1,
  MENU_POLYMODE,
  MENU_TEXTURING,
  MENU_EXIT
};
GLuint programID;
GLuint vertexID;
GLuint normalID;
GLuint textureID;
GLuint ModelID;
GLuint WorldID;
GLuint ViewID;
GLuint LightID;
GLuint CameraID;
glm::vec3 cameraPos(4,3,3), cameraLookAt(0,0,0), cameraUp(0,1,0), lightPos(4,4,4);
GLuint ProjectionID;
GLuint vertexbuffer;
GLuint normalbuffer;
GLuint texturebuffer;
GLuint sampler;
GLuint Texture;
int g_Width = 600, g_Height = 480;
float g_nearPlane = 0.1, g_farPlane = 5000;
struct timeval last_idle_time, time_now;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // Won't be used at the moment.

void RenderObjects(void)
{
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

   glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
   // Or, for an ortho camera :
   //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
  
   // Camera matrix
   glm::mat4 View = glm::lookAt(
                cameraPos,
                cameraLookAt,
                cameraUp
               );
   // Model matrix : an identity matrix (model will be at the origin)
   glm::mat4 World      = glm::mat4(1.0f);
   glm::mat4 Model      = glm::mat4(1.0f);
   glUniformMatrix4fv(ModelID, 1, false, &Model[0][0]);
   glUniformMatrix4fv(WorldID, 1, false, &World[0][0]);
   glUniformMatrix4fv(ViewID, 1, false, &View[0][0]);
   glUniformMatrix4fv(ProjectionID, 1, false, &Projection[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(sampler, 0);
    glUniform3fv(CameraID, 1, &cameraPos[0]);
    glUniform3fv(LightID, 1, &lightPos[0]);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(vertexID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
      vertexID, // The attribute we want to configure
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(normalID);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
      normalID, // The attribute we want to configure
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(textureID);
    glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
    glVertexAttribPointer(
      textureID,               // The attribute we want to configure
      2,                           // size
      GL_FLOAT,                    // type
      GL_FALSE,                    // normalized?
      0,                           // stride
      (void*)0                     // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3 * vertices.size()); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(vertexID);
    glDisableVertexAttribArray(normalID);
    glDisableVertexAttribArray(textureID);
}
void display(void)
{
   // Clear frame buffer and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // Set up viewing transformation, looking down -Z axis
   glLoadIdentity();
   // Render the scene
   RenderObjects();
   // Make sure changes appear onscreen
   glutSwapBuffers();
}
void reshape(GLint width, GLint height)
{
   g_Width = width;
   g_Height = height;
   glViewport(0, 0, g_Width, g_Height);
}
void MouseButton(int button, int state, int x, int y)
{
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
}
void MouseMotion(int x, int y)
{
  // If button1 pressed, zoom in/out if mouse is moved up/down.
}
void Simulate_To_Frame(float t)
{

}
void AnimateScene(void)
{
  float dt;
#ifdef _WIN32
  DWORD time_now;
  time_now = GetTickCount();
  dt = (float) (time_now - last_idle_time) / 1000.0;
#else
  // Figure out time elapsed since last call to idle function
  struct timeval time_now;
//  gettimeofday(&time_now, NULL);
  dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
  1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
  Simulate_To_Frame(dt);
  // Save time_now for next time
  last_idle_time = time_now;
  // Force redraw
  glutPostRedisplay();
}
void SelectFromMenu(int idCommand)
{
  switch (idCommand)
    {
    case MENU_LIGHTING:
      break;
    case MENU_POLYMODE:
      break;      
    case MENU_TEXTURING:
      break;    
    case MENU_EXIT:
      exit (0);
      break;
    }
  // Almost any menu selection requires a redraw
  glutPostRedisplay();
}
void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:             // ESCAPE key
	  exit (0);
	  break;
  case 'l':
	  SelectFromMenu(MENU_LIGHTING);
	  break;
  case 'p':
	  SelectFromMenu(MENU_POLYMODE);
	  break;
  case 't':
	  SelectFromMenu(MENU_TEXTURING);
	  break;
  }
}
int BuildPopupMenu (void)
{
  int menu;
  menu = glutCreateMenu (SelectFromMenu);
  glutAddMenuEntry ("Toggle lighting\tl", MENU_LIGHTING);
  glutAddMenuEntry ("Toggle polygon fill\tp", MENU_POLYMODE);
  glutAddMenuEntry ("Toggle texturing\tt", MENU_TEXTURING);
  glutAddMenuEntry ("Exit demo\tEsc", MENU_EXIT);
  return menu;
}
void initShader() {
    // Create and compile our GLSL program from the shaders
  programID = LoadShaders( "minimal.vert", "minimal.frag" );

  // Get a handle for our buffers
  vertexID = glGetAttribLocation(programID, "vertex");
  normalID = glGetAttribLocation(programID, "normal");
  textureID = glGetAttribLocation(programID, "texture");
  ModelID = glGetUniformLocation(programID, "model");
  WorldID = glGetUniformLocation(programID, "world");
  ViewID = glGetUniformLocation(programID, "view");
  ProjectionID = glGetUniformLocation(programID, "projection");
  LightID = glGetUniformLocation(programID, "light");
  CameraID = glGetUniformLocation(programID, "camera");
}
void initScene() {
  bool res = loadOBJ("cube.obj", vertices, uvs, normals);

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &texturebuffer);
  glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 

  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  // Bind our texture in Texture Unit 0
  Texture = loadBMP_custom("uvtemplate.bmp");
  sampler = glGetUniformLocation(programID, "myTextureSampler");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(sampler, 0);
}

int main(int argc, char** argv)
{
  // GLUT Window Initialization:
  glutInit (&argc, argv);
  glutInitWindowSize (g_Width, g_Height);
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("CS248 GLUT example");
  // Initialize OpenGL graphics state
  // Register callbacks:
  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  glutKeyboardFunc (Keyboard);
  glutMouseFunc (MouseButton);
  glutMotionFunc (MouseMotion);
  glutIdleFunc (AnimateScene);
  // Create our popup menu
  BuildPopupMenu ();
  glutAttachMenu (GLUT_RIGHT_BUTTON);
  // Init shader and scene
  initShader();
  initScene();
  // Get the initial time, for use by animation
  // Turn the flow of control over to GLUT
  glutMainLoop ();
  return 0;
}