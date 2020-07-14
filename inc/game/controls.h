#define MOVESPEED 0.05f
#define MOUSE_SENSITIVITY 0.001f

void InitControls();

void ControlsUpdate();
void OnKey(GLFWwindow* window, int key, int code, int action, int mods);
void OnMouseMove(GLFWwindow* window, double x, double y);
void OnMouseButton(GLFWwindow* window, int button, int action, int mods);