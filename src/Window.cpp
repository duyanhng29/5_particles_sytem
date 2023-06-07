#include "Window.h"
#include "glfw3.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Project 5";


ParticleSystem* Window::particleSys = nullptr;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::particleShaderProgram;


// FPS count
float Window::deltaT = 0;
float Window::prevT;
int Window::countFPS = 0;
int Window::currFPS = 0;


// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	particleShaderProgram = LoadShaders("shaders/shader_particle.vert", "shaders/shader_particle.frag");
	if (!particleShaderProgram)
	{
		std::cerr << "Failed to initialize particle shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{

	particleSys = new ParticleSystem(particleShaderProgram, shaderProgram);

	prevT = glfwGetTime();

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	//delete cube;

	if (particleSys) delete particleSys;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
	glDeleteProgram(particleShaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwSetWindowPos(window, 700, 300);

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	if (height == 0.0f) return;
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}


// update and draw functions
void Window::idleCallback()
{
	// Perform any updates as necessary. 
	Cam->Update();

	if (particleSys != nullptr) particleSys->Update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	//FPS
	float currT = glfwGetTime();
	deltaT += 0.01f;
	prevT = currT;
	countFPS++;
	if (deltaT >= 1000)
	{
		currFPS = countFPS;
		countFPS = 0;
		deltaT -= 1000;
	}

	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	if (particleSys != nullptr)
	{
		particleSys->Draw(Cam->GetViewProjectMtx());
	}

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();

	plotImGUI();

	// Swap buffers.
	glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
		case GLFW_KEY_O:
            Cam->SetDistance((Cam->GetDistance() * 2.5));
            break;
        case GLFW_KEY_I:
            Cam->SetDistance((Cam->GetDistance() / 2.5));
            break;
		case GLFW_KEY_R:
			resetCamera();
			break;

		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

void Window::plotImGUI()
{
	//start imgui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	//gui window
	ImGui::Begin("Coefficients", NULL, window_flags);
	ImGui::SetWindowSize(ImVec2(260, Window::height));

	std::string fps = "FPS: " + std::to_string(currFPS);
	ImGui::Text(fps.c_str());

	if (ImGui::TreeNode("Particle"))
	{
		float r = particleSys->GetRadius();
		if (ImGui::DragFloat("Radius", &r))
		{
			particleSys->SetRadius(r);
		}

		float m = particleSys->GetMass();
		if (ImGui::DragFloat("Mass", &m))
		{
			particleSys->SetMass(m);
		}

		float g = particleSys->GetGravity();
		if (ImGui::DragFloat("g", &g))
		{
			particleSys->SetGravity(g);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Particle Creation"))
	{
		float rate = particleSys->GetCreationRate();
		if (ImGui::DragFloat("Rate", &rate))
		{
			particleSys->SetCreationRate(rate);
		}

		float lifeSpan = particleSys->GetInitialLifeSpan();
		if (ImGui::DragFloat("Life", &lifeSpan))
		{
			particleSys->SetInitialLifeSpan(lifeSpan);
		}

		float lifeSpanVar = particleSys->GetInitialLifeSpanVar();
		if (ImGui::DragFloat("Life Var", &lifeSpanVar))
		{
			particleSys->SetInitialLifeSpanVar(lifeSpanVar);
		}

		glm::vec3 pos = particleSys->GetInitialPos();
		if (ImGui::InputFloat3("Pos", glm::value_ptr(pos)))
		{
			particleSys->SetInitialPos(pos);
		}

		glm::vec3 posVar = particleSys->GetInitialPosVar();
		if (ImGui::InputFloat3("Pos Var", glm::value_ptr(posVar)))
		{
			particleSys->SetInitialPosVar(posVar);
		}

		glm::vec3 v = particleSys->GetInitialVelocity();
		if (ImGui::InputFloat3("V", glm::value_ptr(v)))
		{
			particleSys->SetInitialVelocity(v);
		}

		glm::vec3 v_var = particleSys->GetInitialVelocityVar();
		if (ImGui::InputFloat3("V_var", glm::value_ptr(v_var)))
		{
			particleSys->SetInitialVelocityVar(v_var);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Aerodynamic Force"))
	{
		float rho = particleSys->GetAirDensity();
		if (ImGui::DragFloat("rho", &rho))
		{
			particleSys->SetAirDensity(rho);
		}

		float C_d = particleSys->GetDragConst();
		if (ImGui::DragFloat("C_d", &C_d))
		{
			particleSys->SetDragConst(C_d);
		}

		glm::vec3 V_wind = particleSys->GetWindSpeed();
		if (ImGui::InputFloat3("V_wind", glm::value_ptr(V_wind)))
		{
			particleSys->SetWindSpeed(V_wind);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Ground"))
	{
		glm::vec3 center = particleSys->GetGroundCenter();
		if (ImGui::InputFloat3("Center", glm::value_ptr(center)))
		{
			particleSys->SetGroundCenter(center);
		}

		float size = particleSys->GetGroundSize();
		if (ImGui::DragFloat("Size", &size))
		{
			particleSys->SetGroundSize(size);
		}

		float epsilon = particleSys->GetElasticity();
		if (ImGui::DragFloat("Elasticity", &epsilon))
		{
			particleSys->SetElasticity(epsilon);
		}

		float mu = particleSys->GetFriction();
		if (ImGui::DragFloat("Friction", &mu))
		{
			particleSys->SetFriction(mu);
		}

		ImGui::TreePop();
	}

	ImGui::End();

	//draw imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

