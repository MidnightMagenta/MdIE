#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glad/glad.h>
#include <image.hpp>

static void glfw_err_cb(int code, const char *desc) { std::printf("GLFW error %d: %s\n", code, desc); }

static GLuint compile_shader(GLenum type, const char *src) {
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &src, nullptr);
	glCompileShader(s);
	GLint res = 0;
	glGetShaderiv(s, GL_COMPILE_STATUS, &res);
	if (!res) {
		GLint len = 0;
		glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
		char *log = new char[len];
		glGetShaderInfoLog(s, len, nullptr, log);
		std::printf("Shader compile error: %s\n", log);
		delete[] log;
	}
	return s;
}

static GLuint make_program(const char *vs, const char *fs) {
	GLuint v = compile_shader(GL_VERTEX_SHADER, vs);
	GLuint f = compile_shader(GL_FRAGMENT_SHADER, fs);
	GLuint p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	glDeleteShader(v);
	glDeleteShader(f);

	GLint res = 0;
	glGetProgramiv(p, GL_LINK_STATUS, &res);
	if (!res) {
		GLint len = 0;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
		char *log = new char[len];
		glGetProgramInfoLog(p, len, nullptr, log);
		std::printf("Program link error: %s\n", log);
		delete[] log;
	}
	return p;
}

int main() {
	mdie::Image img(16, 16, sizeof(mdie::rgba_8));
	mdie::ImageView imgView(&img);

	for (size_t i = 0; i < 16; i++) {
		for (size_t j = 0; j < 16; j++) { imgView.get_pixel(i, j) = mdie::rgba_8(i * 16, j * 16, 0, 255); }
	}

	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) { return 1; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow *win = glfwCreateWindow(800, 800, "Image test", nullptr, nullptr);
	if (!win) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::printf("Failed to initialize GLAD\n");
		return 1;
	}

	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, img.m_pitch / img.m_pixelSize);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float verts[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
	};
	uint32_t idx[] = {0, 1, 2, 2, 3, 0};

	GLuint vao = 0, vbo = 0, ebo = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

	const char *vsSrc = R"(#version 330 core
						layout(location=0) in vec2 inPos;
						layout(location=1) in vec2 inUV;
						out vec2 vUV;
						void main(){
							vUV = inUV;
							gl_Position = vec4(inPos, 0.0, 1.0);
						})";

	const char *fsSrc = R"(#version 330 core
						in vec2 vUV;
						out vec4 fragColor;
						uniform sampler2D uTex;
						void main(){
							fragColor = texture(uTex, vUV);
						})";

	GLuint prog = make_program(vsSrc, fsSrc);

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClearColor(0.08f, 0.09f, 0.11f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUseProgram(prog);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(win);
	}

	glDeleteProgram(prog);
	glDeleteTextures(1, &tex);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}