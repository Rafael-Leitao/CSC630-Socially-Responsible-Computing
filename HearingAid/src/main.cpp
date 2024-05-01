/* Lab 6 base code - transforms using matrix stack built on glm
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "Camera.h"

#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Shape to be used (from obj file)
	shared_ptr<Shape> base;
	shared_ptr<Shape> dial;
	shared_ptr<Shape> midButton;
	shared_ptr<Shape> botButton;
	shared_ptr<Shape> stem;
	shared_ptr<Shape> baseToStem;
	shared_ptr<Shape> speaker;
	shared_ptr<Shape> ear;
	//vector<shared_ptr<Shape>> hearingAid;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	float lightTrans = 0;
	float gRot = 0;
	float gTrans = 0;
	float rotAmt = 0;
	float rotInc = 0.01;

	float deg45 = 0.785398;
	float deg90 = 1.5708;
	float deg135 = 2.35619;

	Camera myCamera;
	bool firstMouse = true;
	bool isRightMouseButtonPressed = false;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
		if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
			lightTrans--;
		}
		if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
			lightTrans++;
		}
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			myCamera.moveLeft();
		}
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			myCamera.moveRight();
		}
		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			myCamera.moveForward();
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			myCamera.moveBack();
		}
	}

	void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
		// ... rest of your mouseCallback code
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				isRightMouseButtonPressed = true;
			}
			else if (action == GLFW_RELEASE) {
				isRightMouseButtonPressed = false;
			}
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("MatAmb");
		prog->addUniform("lightP");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
	}

	void initGeom(const std::string& objectDirectory)
	{
		// Initialize mesh.
		dial = make_shared<Shape>();
		base = make_shared<Shape>();
		midButton = make_shared<Shape>();
		botButton = make_shared<Shape>();
		stem = make_shared<Shape>();
		speaker = make_shared<Shape>();
		baseToStem = make_shared<Shape>();
		ear = make_shared<Shape>();
		//shape->loadMesh(resourceDirectory + "/SmoothSphere.obj");
		//shape->loadMesh(resourceDirectory + "/sphere.obj");
		dial->loadMesh(objectDirectory + "/hearingAid/dial.obj");
		dial->resize();
		dial->init();

		base->loadMesh(objectDirectory + "/hearingAid/base.obj");
		base->resize();
		base->init();


		midButton->loadMesh(objectDirectory + "/hearingAid/midButton.obj");
		midButton->resize();
		midButton->init();


		botButton->loadMesh(objectDirectory + "/hearingAid/bottomButton.obj");
		botButton->resize();
		botButton->init();

		//when i saved it in blender i thought the stem and tip were in the same object file, will update
		stem->loadMesh(objectDirectory + "/hearingAid/stemInsert.obj");
		stem->resize();
		stem->init();


		baseToStem->loadMesh(objectDirectory + "/hearingAid/baseConnectStem.obj");
		baseToStem->resize();
		baseToStem->init();


		speaker->loadMesh(objectDirectory + "/hearingAid/speaker.obj");
		speaker->resize();
		speaker->init();

		ear->loadMesh(objectDirectory + "/hearingAid/ear.obj");
		ear->resize();
		ear->init();

		//// init multi shape object
		//vector<tinyobj::shape_t> TOshapes;
		//vector<tinyobj::material_t> objMaterials;
		//string errStr;
		//// loads object into shapes and stores in TOshapes
		//bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (objectDirectory + "/hearingAid/hearingAid.obj").c_str());
		//if (!rc) {
		//	cerr << errStr << endl;
		//}
		//else {
		//	for (int i = 0; i < TOshapes.size(); i++) {
		//		// Initialize each mesh.
		//		shared_ptr<Shape> s;
		//		s = make_shared<Shape>();
		//		s->loadMultiMesh(TOshapes[i]);
		//		s->init();

		//		hearingAid.push_back(s);
		//	}
		//}
	}

		void SetMaterial(int i) {

    		prog->bind();
    		switch (i) {
    		case 0: //shiny blue plastic
    			glUniform3f(prog->getUniform("MatAmb"), 0.02, 0.04, 0.2);
    			/*glUniform3f(prog->getUniform("MatDif"), 0.0, 0.16, 0.9);
    			glUniform3f(prog->getUniform("MatSpec"), 0.14, 0.2, 0.8);
    			glUniform1f(prog->getUniform("MatShine"), 120.0);*/
    		break;
    		case 1: // flat grey
    			glUniform3f(prog->getUniform("MatAmb"), 0.13, 0.13, 0.14);
    			/*glUniform3f(prog->getUniform("MatDif"), 0.3, 0.3, 0.4);
    			glUniform3f(prog->getUniform("MatSpec"), 0.3, 0.3, 0.4);
    			glUniform1f(prog->getUniform("MatShine"), 4.0);*/
    		break;
    		case 2: //brass
    			glUniform3f(prog->getUniform("MatAmb"), 0.3294, 0.2235, 0.02745);
    			/*glUniform3f(prog->getUniform("MatDif"), 0.7804, 0.5686, 0.11373);
    			glUniform3f(prog->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
    			glUniform1f(prog->getUniform("MatShine"), 27.9);*/
    		break;
			case 3: //light green
				glUniform3f(prog->getUniform("MatAmb"), 0.35, 0.5, 0.45);
				/*glUniform3f(prog->getUniform("MatDif"), 0.7804, 0.5686, 0.11373);
				glUniform3f(prog->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
				glUniform1f(prog->getUniform("MatShine"), 27.9);*/
				break;
  		}
	}
		
		//void drawHearingAid(shared_ptr<MatrixStack> Model)
		//{
		//	Model->pushMatrix();
		//	Model->translate(vec3(-0.4, -1, 0));
		//	Model->rotate(deg90, vec3(0, 0, 1));
		//	Model->rotate(deg45, vec3(0, 1, 0));
		//	Model->scale(7);
		//	SetMaterial(1);
		//	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		//	// draw each shape in hearing aid
		//	for (int i = 0; i < hearingAid.size(); i++) {
		//		hearingAid[i]->draw(prog);
		//	}
		//	Model->popMatrix();
		//}
		void drawHearingAid(shared_ptr<MatrixStack> Model)
		{

			//Note: I only made them different materials for easier visualization in the beginning

			Model->pushMatrix();
			//global rotate (the whole scene)
			Model->rotate(gRot, vec3(0, 1, 0));
			int basePosx = 0;
			int basePosy = 0.5;
			int basePosz = -9 + gTrans;
			// draw base
			Model->pushMatrix();
			Model->translate(vec3(basePosx, basePosy, basePosz));
			SetMaterial(1);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			base->draw(prog);
			Model->popMatrix();

			// draw basetoStem
			Model->pushMatrix();
			Model->translate(vec3(basePosx+.45, basePosy + .90, basePosz));
			Model->scale(vec3(0.44, 0.44, 0.44));
			SetMaterial(0);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			baseToStem->draw(prog);
			Model->popMatrix();

			//stem and speaker dome
			Model->pushMatrix(); 
			Model->translate(vec3(basePosx + 1, basePosy+.14 , basePosz));
			Model->scale(vec3(0.66, 0.7, 0.66));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			stem->draw(prog);
			Model->popMatrix(); 

			//Speaker
			Model->pushMatrix();
			Model->translate(vec3(basePosx + 1.12, basePosy -.23, basePosz));
			Model->scale(vec3(0.26, 0.3, 0.3));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			speaker->draw(prog);
			Model->popMatrix();

			//dial
			Model->pushMatrix();
			Model->translate(vec3(basePosx -.275, basePosy + .5, basePosz));
			Model->scale(vec3(0.2, 0.2, 0.2));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			dial->draw(prog);
			Model->popMatrix();

			//Middle button
			Model->pushMatrix(); 
			Model->translate(vec3(basePosx - .4, basePosy + .1, basePosz));
			Model->scale(vec3(0.07, 0.07, 0.07));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			midButton->draw(prog);
			Model->popMatrix();

			//bottom Button
			Model->pushMatrix(); 
			Model->translate(vec3(basePosx - .45, basePosy -.4, basePosz));
			Model->scale(vec3(0.07, 0.07, 0.07));
			SetMaterial(2);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			botButton->draw(prog);
			Model->popMatrix();

			//Ear
			Model->pushMatrix();
			Model->translate(vec3(basePosx+.4, basePosy, basePosz-.3));
			Model->scale(vec3(1, 1, 1));
			Model->rotate(90, vec3(0,1,0));
			SetMaterial(3);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			ear->draw(prog);
			Model->popMatrix();
			
			
		}

	void render()
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the matrix stack for Lab 6
		float aspect = width/(float)height;

		// Create the matrix stacks - please leave these alone for now
		auto Projection = make_shared<MatrixStack>();
		auto View = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();

		// Apply perspective projection.
		Projection->pushMatrix();
		Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

		// View is identity - for now
		View->pushMatrix();

		View->loadIdentity();

		View->multMatrix(myCamera.getViewMatrix());

		// Draw a stack of cubes with indiviudal transforms
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniform3f(prog->getUniform("lightP"), lightTrans+2, 3, 5);

		// draw the spheres
		Model->pushMatrix();

		//global rotate (the whole scene )
		Model->rotate(gRot, vec3(0, 1, 0));

		drawHearingAid(Model);
		
		prog->unbind();

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";
	std::string objectDir = "../objects";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(objectDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		if (application->isRightMouseButtonPressed) {
			double posX, posY;
			glfwGetCursorPos(windowManager->getHandle(), &posX, &posY);
			application->myCamera.lookAround(windowManager->getHandle(), posX, posY);
		}

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
