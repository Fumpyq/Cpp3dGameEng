
#pragma region DEFINES
//
#define MultuThreadTexLoad
#define Profiler
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define Debug
#pragma endregion

#pragma region INCLUDES

#include "Lirbarys/imGui/imgui.h"
#include "Lirbarys/imGui/imgui_impl_glfw.h"
#include "Lirbarys/imGui/imgui_impl_opengl3.h"
//#include "Lirbarys/FMOD/fmod.hpp"
//#include "Lirbarys/FMOD/phonon.h"

#include <atomic>
#include <semaphore>
#include <thread>
#include <mutex>
#include <stddef.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vertex.h"
#include "PRCMesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#ifdef Debug
#include <iostream>
#include <sstream>
#endif
#include <unordered_set>
//#include <cmath> 

#include "Lirbarys/stb_image.h"
#include <iterator>
#include <queue>
#include <string> 
#include <map>

#include <fstream>
#include <chrono>
#include <windows.h>
#include <vector>


#include "windows.h"
#include "psapi.h"



#ifdef __APPLE__
#include <Availability.h> // for deployment target to support pre-catalina targets without std::fs 
#endif
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (defined(__cplusplus) && __cplusplus >= 201703L)) && defined(__has_include)
#if __has_include(<filesystem>) && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define GHC_USE_STD_FS
#include <filesystem>
namespace fs = std::filesystem;
#endif
#endif
#ifndef GHC_USE_STD_FS
#include "Lirbarys/filesystem.hpp"
namespace fs = ghc::filesystem;
#endif
using namespace std;
using std::vector;
#pragma endregion



#pragma region ENUMS
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
enum CompressionMethod
{
	None = 1,
	Fast = 4,
	Long = 8,
};
enum TextureType
{
	albedo,
	normal,
	metalic,
	specular,
	ao,
	height,
	emmmisive,
	coatmasc,
};
enum RenderShader
{
	SCREEN = 0,
	DEPTH = 1,
};
#pragma endregion

GLuint CurrentShaderType = 0;

#pragma region Supports
#ifdef Profiler
float VerCount;
float TrisCount;
#endif
#pragma region Consts
const char* ShaderParamError = "try to set unused shader param";
const GLfloat byte2Mb = 1048576.0;
const std::string OpenGlShaderVersion = "#version 460 core";
const short OpenGlVersionMa = 4;
const short OpenGlVersionMi = 6;
const glm::vec3 vec3Up = glm::vec3(0, 1, 0);
std::string AppPath = fs::current_path().string();
ConsoleColor CurrentColor;
GLfloat Pi = 3.1416f;
GLfloat Pi2 = Pi / 2;
#pragma endregion

#pragma region Methods
void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hCon;
	CurrentColor = text;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, (WORD)((background << 4) | text));
};
#ifdef Debug
void LogTotalTime(std::chrono::steady_clock::time_point start)
{
	auto finish = std::chrono::high_resolution_clock::now();
	GLfloat CompileTimeSec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / 1000000000.0;
	if (CompileTimeSec > 0.1f)
		std::cout << "\nTotal: " << round(CompileTimeSec * 100) / 100 << " s.\n";   //Compilation Time
	else
		std::cout << "\nTotal: " << CompileTimeSec << " s.\n";   //Compilation Time
};

void LogMatrix(glm::mat4& mat, const char* matname)
{
	std::cout << "=====" << matname << "=====" << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat[0][0], mat[0][1], mat[0][2], mat[0][3]) << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat[1][0], mat[1][1], mat[1][2], mat[1][3]) << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat[2][0], mat[2][1], mat[2][2], mat[2][3]) << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat[3][0], mat[3][1], mat[3][2], mat[3][3]) << std::endl;
}
void LogMatrix(glm::mat4* mat, const char* matname)
{
	std::cout << "=====" << matname << "=====" << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat->operator[](0)[0], mat->operator[](0)[1], mat->operator[](0)[2], mat->operator[](0)[3]) << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat->operator[](1)[0], mat->operator[](1)[1], mat->operator[](1)[2], mat->operator[](1)[3]) << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat->operator[](2)[0], mat->operator[](2)[1], mat->operator[](2)[2], mat->operator[](2)[3]) << std::endl;
	std::cout << std::format("{}\t|{}\t|{}\t|{}", mat->operator[](3)[0], mat->operator[](3)[1], mat->operator[](3)[2], mat->operator[](3)[3]) << std::endl;
}
#endif																																	
int RandomInt(int min, GLfloat max)
{
	//std::srand(std::time(0));
	return  min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
};

GLfloat RandomGLf(GLfloat min, GLfloat max, GLubyte precision)
{
	//std::srand(std::time(0));
	setprecision(precision);
	return  min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
};
glm::vec3 Randomv3(GLfloat min, GLfloat max)
{
	return glm::vec3(RandomGLf(min, max, 2), RandomGLf(min, max, 2), RandomGLf(min, max, 2));
};

glm::vec3 Randomv3(glm::vec3 min, glm::vec3 max)
{
	return glm::vec3(RandomGLf(min.x, max.x, 2), RandomGLf(min.y, max.y, 2), RandomGLf(min.z, max.z, 2));
};

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindVertexArray(0);
};
#pragma endregion

#pragma endregion
#pragma region STRUCTS_TEMPLATES

#pragma region STRUCTS
struct Transform
{
public:
	int id;
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::quat qRot = { 0.0f, 0.0f, 0.0f ,0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	bool isDirty;

	vector<glm::mat4>* modelMatrixPtr;
public:
	void SetPosition(glm::vec3& newPosition)
	{
		position = newPosition;
		isDirty = true;
	}
	void AddPosition(glm::vec3 addPosition)
	{
		position += addPosition;
		isDirty = true;
	}
	glm::mat4* GetMatrixPtr()
	{
		return &modelMatrixPtr->operator[](id);
	}
	void RecalcMatrix()
	{
		//if (!isDirty)return;
		isDirty = false;
		float* qw = &qRot.x;
		float* qx = &qRot.y;
		float* qy = &qRot.z;
		float* qz = &qRot.w;
		const float n = 1.0f / sqrt(*qx * *qx + *qy * *qy + *qz * *qz + *qw * *qw);
		*qx *= n;
		*qy *= n;
		*qz *= n;
		*qw *= n;

		//modelMatrix->operator=(glm::mat4
		//(1., 0., 0., position.x,
		//	0., 1., 0., position.y,
		//	0., 0., 1., position.z,
		//	0., 0., 0., 1.));
		glm::mat4* m =&modelMatrixPtr->operator[](id);
		//glm::mat4 RotationMatrix = glm::::toMat4(quaternion);
		m->operator=(glm::translate(glm::mat4(1.0f), position));
		/*GL TransformBuffers[this]*/
	/*	m->operator=(glm::rotate(glm::mat4(1.0f), qRot.x));*/
		//glm::mat4(
		//	1.0f - 2.0f * *qy * *qy - 2.0f * *qz * *qz, 2.0f * *qx * *qy - 2.0f * *qz * *qw, 2.0f * *qx * *qz + 2.0f * *qy * *qw, 0.0f,
		//	2.0f * *qx * *qy + 2.0f * *qz * *qw, 1.0f - 2.0f * *qx * *qx - 2.0f * *qz * *qz, 2.0f * *qy * *qz - 2.0f * *qx * *qw, 0.0f,
		//	2.0f * *qx * *qz - 2.0f * *qy * *qw, 2.0f * *qy * *qz + 2.0f * *qx * *qw, 1.0f - 2.0f * *qx * *qx - 2.0f * *qy * *qy, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f)
		//*

	}
};

struct Noise
{
private:
	vector<int> p;
	vector<int> tmp = { 151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };
	int _halfLength;
	float Grad(int hash, float x, float y, float z)
	{
		int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE

		float u = h < 8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;

		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}
	float Fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	float Lerp(float a, float b, float dt) { return a + (b - a) * dt; }
	void GenerateBySeed(uint32_t seed)
	{


		_halfLength = tmp.capacity() / sizeof(int);
		p.resize(_halfLength * 2 + 1);
		for (int i = 0; i < _halfLength * 2; i++)
		{
			p[i] = 228;
		}

		int c = _halfLength;



		for (int i = 0; i < c; i++)
		{
			int swapIndex = RandomInt(0, _halfLength);

			int t = tmp[i];

			tmp[i] = tmp[swapIndex];

			tmp[swapIndex] = t;

			p[_halfLength + i] = p[i] = tmp[i];
			p[swapIndex + _halfLength] = p[swapIndex] = tmp[swapIndex];
		}
	}
public:
	float Get(float x, float y, float z, int octaves/*, ref float min, ref float max*/)
	{

		float perlin = 0;
		float octave = 1;

		for (int i = 0; i < octaves; i++)
		{
			float noise = Get(x * octave, y * octave, z * octave);

			perlin += noise / octave;

			octave *= 2;
		}

		perlin = abs((float)pow(perlin, 2));
		//max = Mathf.Max(perlin, max);
		//min = Mathf.Min(perlin, min);

		//perlin = 1f - 2 * perlin;

		return perlin;
	}
	float Get(float x, float y, float z)
	{



		int X = (int)floor(x) % _halfLength;
		int Y = (int)floor(y) % _halfLength;
		int Z = (int)floor(z) % _halfLength;

		if (X < 0)
			X += _halfLength;

		if (Y < 0)
			Y += _halfLength;

		if (Z < 0)
			Z += _halfLength;

		x -= (int)floor(x);
		y -= (int)floor(y);
		z -= (int)floor(z);

		float u = Fade(x);
		float v = Fade(y);
		float w = Fade(z);

		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
			B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

		return lerp(
			lerp(
				lerp(
					Grad(p[AA], x, y, z) // AND ADD
					,
					Grad(p[BA], x - 1, y, z) // BLENDED
					,
					u
				)
				,
				lerp(
					Grad(p[AB], x, y - 1, z)  // RESULTS
					,
					Grad(p[BB], x - 1, y - 1, z)
					,
					u
				)
				,
				v
			)
			,
			lerp(
				lerp(
					Grad(p[AA + 1], x, y, z - 1) // CORNERS
					,
					Grad(p[BA + 1], x - 1, y, z - 1) // OF CUBE
					,
					u
				)
				,
				lerp(
					Grad(p[AB + 1], x, y - 1, z - 1)
					,
					Grad(p[BB + 1], x - 1, y - 1, z - 1)
					,
					u
				)
				,
				v
			)
			,
			w
		);

	}
	/// implements improved Perlin noise in 2D. 
	/// Transcribed from http://www.siafoo.net/snippet/144?nolinenos#perlin2003
	/// </summary>
	float Get2D(float x, float y, int octaves, float Md)
	{
		//if (x == 0) x = 0.001f;
		//if (y == 0) y = 0.001f;
		float perlin = 0;
		float octave = 1;

		for (int i = 0; i < octaves; i++)
		{
			perlin += Get2D(x * octave, y * octave, Md) / octave;

			octave *= 2;
		}

		perlin = abs((float)pow(perlin, 2));
		//max = Mathf.Max(perlin, max);
		//min = Mathf.Min(perlin, min);

		//perlin = 1f - 2 * perlin;

		return perlin;
	}
	float Get2D(float x, float y, float Md)
	{

		// Find unit square that contains point.
		int X = (int)floor(x) % _halfLength;
		int Y = (int)floor(y) % _halfLength;

		if (X < 0)
			X += _halfLength;

		if (Y < 0)
			Y += _halfLength;


		x -= (int)floor(x);
		y -= (int)floor(y);


		float u = Fade(x);


		return Lerp(
			Lerp(
				Grad(p[p[X] + Y], x, y, Md) // AND ADD
				,
				Grad(p[p[X + 1] + Y], x - 1, y, Md) // BLENDED
				,
				u
			)
			,
			Lerp(
				Grad(p[p[X] + Y + 1], x, y - 1, Md)  // RESULTS
				,
				Grad(p[p[X + 1] + Y + 1], x - 1, y - 1, Md)
				,
				u
			)
			,
			Fade(y)
		);
	}
	Noise(uint32_t seed)
	{
		GenerateBySeed(seed);
	}
	~Noise()
	{
		p.~vector();
	}
};

#pragma endregion
#pragma region TEMPLATES

template<typename  Key, typename Value>
class Dictionary
{
public:
	map<Key, Value> data;
public:
	Value& operator [](const Key key)
	{
		return data[key];
	}
	bool contains(const Key key)
	{
		return data.contains(key);
	}
	GLuint count()
	{
		return data.size();
	}
public:
	Dictionary()
	{
		data = map<Key, Value>();
	}
};
template<typename T>
class Array {
public:
	vector <T> data;

	int count;

	void Add(T t) { data.push_back(t); count++; }
	//void Remove(T t) { data.remove(t); count--; }


	const T& operator [](int index) const
	{
		return data[index];
	}

};
#pragma endregion


#pragma endregion


#ifdef MultuThreadTexLoad

HGLRC hglrc;

HDC hdc;
HGLRC Shglrc;

HDC Shdc;
atomic<int> TexturesToLoad;
std::mutex gl_lock;


#endif
//#define Debug
#pragma region Texture
class Texture2d
{
public:
	GLuint Id;
	GLuint ReallId;
	bool IsOK;
	int32_t textureWeight;
	string path;
	TextureType type;
	CompressionMethod Compression;
public:
	Texture2d(std::string LocalPath, TextureType type, CompressionMethod CompMet = Fast)
	{
		textureWeight = 0;
		this->type = type;
		path = LocalPath;
		Compression = CompMet;
		glCreateTextures(GL_TEXTURE_2D, 1, &ReallId);
		Id = 1;

		/*	glBindTexture(GL_TEXTURE_2D, Id);*/
		glObjectLabel(GL_TEXTURE, ReallId, -1, std::format("ID: {}, {}", ReallId, path.c_str()).c_str());
#ifndef MultuThreadTexLoad
		Load(true);
#endif
	}
	void Load(bool IsMainThread)
	{
#if defined(Debug) && !defined(MultuThreadTexLoad)
		auto start = std::chrono::high_resolution_clock::now();
		std::cout << "zip:" << Compression << "\t|\t\t\t\t\t\t\t\t" << path;
#endif
		int width, height, nrChannels;

		unsigned char* data = stbi_load((AppPath + path).c_str(), &width, &height, &nrChannels, 0);
#if defined(Debug) && !defined(MultuThreadTexLoad)
		auto finish = std::chrono::high_resolution_clock::now();
		GLfloat CompileTimeSec = 0;
		CompileTimeSec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / 1000000000.0;

		std::cout << "\nLoad Data In\t" << round(CompileTimeSec * 100) / 100 << "\t s. ";   //Compilation Time
#endif



		if (data)
		{

#if defined(Debug) && !defined(MultuThreadTexLoad)
			start = std::chrono::high_resolution_clock::now();
#endif		
#ifdef MultuThreadTexLoad
			if (!IsMainThread)
			{
				gl_lock.lock(); wglMakeCurrent(hdc, Shglrc);
			}
#endif

			glTextureParameteri(ReallId, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(ReallId, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureParameteri(ReallId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(ReallId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			if (Compression != None)
			{


				switch (Compression)
				{
				case Fast:
					glTextureImage2DEXT(ReallId, GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					break;
				case Long:
					glTextureImage2DEXT(ReallId, GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					break;
				default:
					break;
				}
#ifdef MultuThreadTexLoad
				if (!IsMainThread)
				{
					glFinish(); Id = ReallId; gl_lock.unlock(); wglMakeCurrent(NULL, NULL);
				}
				else { Id = ReallId; }
#else 
				Id = ReallId;
#endif
				//GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
				// //===========================
				// 1-256x256   24 color
				// 2-2048x2048 24
				// 3-2560x1600 24
				// 4-4096x4096 8
				//                                     |   Time    |   ||   Size    ||   ||Visible
				//                                     | 1 | 2 | 3 | 4 || 1 | 2 | 3 || 4 ||          
				// GL_COMPRESSED_RGBA_S3TC_DXT1_EXT    |.03|1.7|0.3|1.1||.03|2.0|2.0||8.0||      TRUE 
				// GL_COMPRESSED_RGBA_S3TC_DXT5_EXT    |.01|0.4|0.3|1.1||.06|4.0|3.9||16.||      TRUE 
				// GL_COMPRESSED_RGBA_S3TC_DXT3_EXT    | 0 |0.3|0.2|0.8||.06|4.0|3.9||16.||    Sometimes ----Для скорости и не больших текстур
				// =======================================================================================
				// GL_COMPRESSED_RGB                   |.02|1.6|0.4|1.1||.03|2.0|2.0||8.0||    Sometimes ----Для меньшего размера
				// GL_COMPRESSED_RGBA_ARB              |.01|0.4|0.3|1.1||.06|4.0|3.9||16.||      FALSE
				// 
				//             GL_COMPRESSED_RGB8_ETC2          Хз
				// GL_COMPRESSED_RGBA8_ETC2_EAC                       КРАШ  
				// GL_COMPRESSED_RGBA_BPTC_UNORM                      КРАШ  
				// GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB          КРАШ
				//GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT               КРАШ  
				// GL_COMPRESSED_INTENSITY_ARB                        КРАШ   
				// GL_COMPRESSED_RGBA_FXT1_3DFX                       КРАШ 
				// GL_COMPRESSED_RGBA_ASTC_4x4_KHR                    КРАШ 
				// GL_COMPRESSED_RGBA_ASTC_8x8_KHR                    КРАШ   
				// GL_RGB4                                            КРАШ  
				// GL_RGB5                                            КРАШ 
				// GL_RGB
				//Проверим - смогли ли мы сжать текстуру  
//				int compressed;
//				glGetTextureLevelParameteriv(Id, 0, GL_TEXTURE_COMPRESSED_ARB,
//					&compressed);
//				if (compressed != GL_TRUE) //Сжать текстуру не удалось      
//				{
//#if defined(Debug) && !defined(MultuThreadTexLoad)
//					SetColor(LightRed, Black);
//					std::cout << "FAILED" << std::endl;
//					SetColor(White, Black);
//#endif
//					glTextureImage2DEXT(Id, GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//					stbi_image_free(data);
//
//					IsOK = false;
//				}
//				else
//				{
//					glGetTextureLevelParameteriv(Id, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
//						&textureWeight);
//					char* compressed_texture = new char[textureWeight];
//
//					//glCompressedTextureSubImage2D(Id, 0, 0, 0, width, height, GL_TEXTURE_COMPRESSED_ARB, textureWeight, compressed_texture);
//					stbi_image_free(data);
//				}
				stbi_image_free(data);
			}
			else
			{
				glTextureImage2DEXT(ReallId, GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
#ifdef MultuThreadTexLoad
				if (!IsMainThread)
				{
					glFinish(); Id = ReallId; wglMakeCurrent(NULL, NULL); gl_lock.unlock();
				}
				else { Id = ReallId; }
#else 
				Id = ReallId;
#endif

				//glGenerateMipmap(GL_TEXTURE_2D);
				textureWeight = width * height * 4;
				stbi_image_free(data);
			}
			IsOK = true;
#pragma region DebugInfo
#if defined(Debug) && !defined(MultuThreadTexLoad)
			finish = std::chrono::high_resolution_clock::now();
			GLfloat CompileTimeSec = 0;
			CompileTimeSec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / 1000000000.0;

			std::cout << "\nCompressed In\t" << round(CompileTimeSec * 100) / 100 << "\t s. ";   //Compilation Time
			std::cout << " [Size: " << round((GLfloat)textureWeight / byte2Mb * 100) / 100 << "Mb" << "\tw: " << width << "\th: " << height << " ]";//Texture Weight
			std::cout << "\tId: " << Id;                                             //MainId
			SetColor(LightGreen, Black);
			std::cout << "\tOK!" << std::endl;
#endif
#pragma endregion
		}
		else
		{
			IsOK = false;
#if defined(Debug) && !defined(MultuThreadTexLoad)
			SetColor(LightRed, Black);
			std::cout << "\tTexture not found" << std::endl;
			SetColor(White, Black);
			std::cout << "FullPath:\t\t\t\t\t\t\t\t" << (AppPath + path) << std::endl;
#endif

		}

#if defined(Debug) && !defined(MultuThreadTexLoad)
		SetColor(White, Black);
#endif
	}
};
static class DeffaultTexture
{
public:
	static std::optional < Texture2d> MissingTexture;
	static void init()
	{
		MissingTexture = Texture2d("\\Resources\\Images\\MissingTexture.jpg", albedo, Long);
		MissingTexture->Load(true);
	}
};
std::optional <Texture2d> DeffaultTexture::MissingTexture;
#pragma endregion
#pragma region MultiThreading
class Worker
{
protected:
	std::chrono::steady_clock::time_point Timer;
	void StartExecutionTimer() { Timer = std::chrono::high_resolution_clock::now(); }
	void EndExecutionTimer() { msUsage = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - Timer).count() / 1000000.0; }
	void NullExecutionTimer() { msUsage = -1; }
public:
	virtual string GetName() { return "Worker"; };
	float msUsage;
	Worker() {}
public:
	virtual void Work() {}
};
#ifdef MultuThreadTexLoad
class TextureWorker : public Worker
{
	std::binary_semaphore Semaphore = std::binary_semaphore(0);
	std::queue<Texture2d*> ToLoad;
	atomic<int> ToLoadCount;
public:
	void AddTextureToLoad(Texture2d* t)
	{
		ToLoadCount++;
		ToLoad.push(t);
		Semaphore.release();
	}
	TextureWorker() :Worker() {};
	void Work() override
	{

		while (true)
		{
			if (ToLoadCount == 0) {
				NullExecutionTimer(); Semaphore.acquire();
				//
			}
			while (ToLoadCount > 0)
			{

				StartExecutionTimer();
				Texture2d* t = ToLoad.front();
				ToLoad.pop();
				t->Load(false);
				EndExecutionTimer();
				ToLoadCount--;
				TexturesToLoad--;
			}
		}
	}
};
#endif
class MultiThreading
{
public:

	vector<jthread> ScriptThreads = vector<jthread>();
public:
#ifdef MultuThreadTexLoad
	vector <jthread> TextureLoadThread;
	vector <TextureWorker*> worker = vector < TextureWorker*>();
	int n = 0;
#endif

	void init()
	{
#ifdef MultuThreadTexLoad
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());
		worker.push_back(new TextureWorker());

		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[0]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[1]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[2]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[3]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[4]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[5]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[6]));
		TextureLoadThread.push_back(std::jthread(&TextureWorker::Work, worker[7]));
#endif
	}
#ifdef MultuThreadTexLoad
	void LoadTexture(Texture2d* t)
	{
		TexturesToLoad++;
		worker[n]->AddTextureToLoad(t);
		n++;
		if (n > 7)n = 0;
	}
#endif
};

MultiThreading Parallel = MultiThreading();
#pragma endregion
#pragma region Shaders
unsigned int DeffaultDepthShader = 0;
class Shader
{
public:
	//std::map<const char*, Texture2d> textures;
protected:
	virtual bool IsDepthShaderOverrided() { return false; };
	virtual bool IsHaveDepthShader() { return true; };
	//virtual bool IsGBufferOverrided() { return false; };
public:
	//  Id of Shader OpenGl Program (glUseProgram(MainId)); 
	//Dictionary<string, int> Uniforms;
	//bool IsUniformsInited;
	uint32_t textureCount;
	uint32_t MainId;
	//uint32_t ShadowId;
	//uint32_t GBufferId;
	vector<uint32_t> Variarions;
	inline virtual std::string DepthVertex() {
		return std::format(R"({}
layout(location = 0) in vec3 aPos;
layout(location = 3) in mat4 Transform;

uniform mat4 projViewMatrix;
void main()
{{
gl_Position = projViewMatrix * Transform * vec4(aPos, 1.0);
}})",

OpenGlShaderVersion);
	}
	inline virtual std::string DepthFragment() {
		return std::format(R"({}
out vec4 FragColor;
void main()
{{
FragColor = vec4(0.5,0.5,0.5, 1.0);
}})",

OpenGlShaderVersion);
	}
	//inline virtual std::string GBufferVertex() {
	//	return R"glsl(#version 330 core
	//	layout (location = 0) in vec3 aPos;
	//	void main(){
	//	   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//	})glsl";
	//}
	//inline virtual std::string GBufferFragment() {
	//	return R"glsl(#version 330 core
	//	out vec4 FragColor;
	//	void main()
	//	{
	//	   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//	})glsl";
	//}
	inline virtual std::string GetVertexCode() {
		return R"glsl(#version 330 core
		layout (location = 0) in vec3 aPos;
		void main(){
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		})glsl";
	}
	inline virtual std::string GetFragmentCode() {
		return R"glsl(#version 330 core
		out vec4 FragColor;
		void main()
		{
		   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		})glsl";
	}
	virtual const char* vertexShaderSrc()
	{
		return R"glsl(#version 150 core
		in vec2 position;
		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
		})glsl";
	}
public:
	Shader() {
		MainId = 0;
		//ShadowId = 0;
		//GBufferId = 0;
	}
	inline void Compile(bool debug = false)
	{
		SetColor(LightCyan, Black);
#ifdef Debug 
		std::cout << "Shader(" << typeid(*this).name() << ")\n";
#endif
#pragma region Main
		std::string vs = GetVertexCode();
		std::string fs = GetFragmentCode();
		const char* vShaderCode = vs.c_str();
		const char* fShaderCode = fs.c_str();
#ifdef Debug 
		if (debug)
		{
			std::cout << "-- --------------------------------------------------- --\n";
			std::cout << "Vertex Shader Code:\n===========================\n";
			SetColor(Cyan, Black);
			std::cout << vShaderCode << "\n===========================\n";

			SetColor(LightCyan, Black);

			std::cout << "Fragment Shader Code:\n===========================\n";
			SetColor(Cyan, Black);
			std::cout << fShaderCode << "\n===========================\n";
			std::cout << "-- --------------------------------------------------- --\n";
		}
#endif

		SetColor(LightCyan, Black);

		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// shader Program
		MainId = glCreateProgram();
		glAttachShader(MainId, vertex);
		glAttachShader(MainId, fragment);
		glLinkProgram(MainId);
		checkCompileErrors(MainId, "PROGRAM");

#ifdef Debug 
		std::cout << "Compiled MAIN_Id : " << MainId << "\n";
		if (CurrentColor == LightRed) {
			SetColor(White, Black); std::cout << "Status: "; SetColor(LightRed, Black);  std::cout << "Failed" << "\n";
		}
		else {
			std::cout << "Status: "; SetColor(LightGreen, Black);  std::cout << "OK" << "\n";
		}
#endif
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		Variarions.push_back(MainId);
#pragma endregion

#pragma region Depth
		if (!IsHaveDepthShader())
		{
#ifdef Debug 
			SetColor(LightCyan, Black);
			SetColor(White, Black);
#endif
			return;
		}
		if (!IsDepthShaderOverrided())
		{
			if (DeffaultDepthShader != 0)
			{
#ifdef Debug 
				SetColor(LightCyan, Black);
				SetColor(White, Black);
#endif
				Variarions.push_back(DeffaultDepthShader);
				return;
			}
		}

		vs = DepthVertex();
		fs = DepthFragment();
		vShaderCode = vs.c_str();
		fShaderCode = fs.c_str();
#ifdef Debug 
		if (debug)
		{
			std::cout << "-- --------------------------------------------------- --\n";
			std::cout << "Vertex Shader Code:\n===========================\n";
			SetColor(Cyan, Black);
			std::cout << vShaderCode << "\n===========================\n";

			SetColor(LightCyan, Black);

			std::cout << "Fragment Shader Code:\n===========================\n";
			SetColor(Cyan, Black);
			std::cout << fShaderCode << "\n===========================\n";
			std::cout << "-- --------------------------------------------------- --\n";
		}


		SetColor(LightCyan, Black);
#endif
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// shader Program
		int ShadowId = glCreateProgram();
		glAttachShader(ShadowId, vertex);
		glAttachShader(ShadowId, fragment);
		glLinkProgram(ShadowId);
		checkCompileErrors(ShadowId, "PROGRAM");

#ifdef Debug 
		std::cout << "Compiled DEPTH_Id : " << ShadowId << "\n";
		if (CurrentColor == LightRed) {
			SetColor(White, Black); std::cout << "Status: "; SetColor(LightRed, Black);  std::cout << "Failed" << "\n";
		}
		else {
			std::cout << "Status: "; SetColor(LightGreen, Black);  std::cout << "OK" << "\n";
		}		
		SetColor(LightCyan, Black);
		SetColor(White, Black);
#endif
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		Variarions.push_back(ShadowId);

#pragma endregion
	}
	virtual inline void init() {};
	void use()
	{
		glUseProgram(Variarions[CurrentShaderType]);
	}

	// utility uniform functions
	//     // ------------------------------------------------------------------------

private:
	bool checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				SetColor(LightRed, Black);
#ifdef Debug 
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
#endif				
				MainId = 3;
				return true;

			}
			return false;
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				SetColor(LightRed, Black);
#ifdef Debug 
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
#endif	
				MainId = 3;
				return true;
			}
			return false;
		}
	}
public:
#pragma region Shader_SetFuncs


	inline  void setBool(const char name[], int value)
	{

		glUniform1i(glGetUniformLocation(MainId, name), value);

	}
	//------------------------------------------------------------------------
	inline void setBool(const char name[], bool value)
	{
		glUniform1i(glGetUniformLocation(MainId, name), (int)value);
	}
	// ------------------------------------------------------------------------
	inline void setTexture(const char name[], Texture2d* value)
	{
		glUniform1i(glGetUniformLocation(MainId, name), value->Id);
	}
	inline void setInt(const char name[], int value)
	{
		glUniform1i(glGetUniformLocation(MainId, name), value);
	}
	inline void  dynamicSetInt(const char name[], int value)
	{
		glUniform1i(glGetUniformLocation(MainId, name), value);
	}
	// ------------------------------------------------------------------------
	inline void dynamicSetFloat(const char name[], GLfloat value)
	{
		glUniform1f(glGetUniformLocation(MainId, name), value);
	}
	inline void setFloat(const char name[], GLfloat value)
	{

		glUniform1f(glGetUniformLocation(MainId, name), value);

	}
	inline void setVec2(const char name[], const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(MainId, name), 1, &value[0]);
	}
	inline void setVec2(const char name[], GLfloat x, GLfloat y)
	{
		glUniform2f(glGetUniformLocation(MainId, name), x, y);
	}
	// ------------------------------------------------------------------------
	inline void setVec3(const char name[], const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(MainId, name), 1, &value[0]);
	}
	inline void setVec3(const char name[], GLfloat x, GLfloat y, GLfloat z)
	{
		glUniform3f(glGetUniformLocation(MainId, name), x, y, z);
	}
	// ------------------------------------------------------------------------
	inline void setVec4(const char name[], const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(MainId, name), 1, &value[0]);
	}
	inline void setVec4(const char name[], GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(MainId, name), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	inline void setMat2(const char name[], const glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(MainId, name), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	inline void setMat3(const char name[], const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(MainId, name), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	inline void setMat4(const char name[], const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(MainId, name), 1, GL_FALSE, &mat[0][0]);
	}

#pragma endregion


};

class LightShader :public Shader
{
	inline std::string GetVertexCode() override
	{

		return R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 UV;
void main()
{UV = aTexCoords;
gl_Position = vec4(aPos, 1.0);})glsl";
	}
	inline std::string GetFragmentCode() override
	{
		return R"(#version 330 core
out vec4 FragColor;
in vec2 UV;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
struct Light {
vec3 Position;
vec3 Color;
float Linear;
float Quadratic;
float Radius;
};
const int NR_LIGHTS = 4;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;
void main()
{
	vec3 FragPos = texture(gPosition, UV).rgb; 
	vec3 Normal = texture(gNormal, UV).rgb;
	vec3 Diffuse = texture(gAlbedoSpec, UV).rgb; 
	float Specular = texture(gAlbedoSpec, UV).a;

	// then calculate lighting as usual 
	vec3 lighting = Diffuse * 0.1; // hard-coded ambient component
	vec3 viewDir = normalize(viewPos - FragPos);
	for (int i = 0; i < NR_LIGHTS; ++i) 
	{
		//calculate distance between light source and current fragment 
		float distance = length(lights[i].Position - FragPos);
		if (distance < lights[i].Radius)
		{
			// diffuse 
			vec3 lightDir = normalize(lights[i].Position - FragPos);
			vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
			// specular 
			//vec3 halfwayDir = normalize(lightDir + viewDir);
			//float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0); 
			//vec3 specular = lights[i].Color * spec * Specular;

			float attenuation =max( 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance),0.01); 
			diffuse *=attenuation; 
			//specular *= attenuation; 
			lighting += diffuse /*+ specular*/; 
		}
	}
    FragColor = vec4(lighting, 1.0);
})";

	}
};

class Shader3d :public Shader
{
protected:
	virtual inline std::string VVar()
	{
		return"";
	}
	virtual inline std::string VMain()
	{
		return"";
	}
	virtual inline std::string FVar()
	{
		return"";
	}
	virtual inline std::string FMain()
	{
		return " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n";
	}

	virtual inline std::string ShadowFVar()
	{
		return"";
	}
	virtual inline std::string ShadowFMain()
	{
		return " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n";
	}
public:
	inline std::string GetVertexCode() override
	{
		return std::format(R"({}
layout(location = 0) in vec3 aPos;
layout(location = 3) in mat4 Transform;

uniform mat4 projViewMatrix;
{}
void main()
{{
gl_Position = projViewMatrix * Transform * vec4(aPos, 1.0);
{}
}})",

OpenGlShaderVersion, VVar(), VMain());
	}
	inline std::string GetFragmentCode() override
	{
		return std::format(R"({}
out vec4 FragColor;
{}
void main()
{{
{}
}})",

OpenGlShaderVersion, FVar(), FMain());
	}
	inline bool  IsHaveDepthShader()  override
	{
		return true;
	}
	//	inline std::string DepthVertex() override
	//	{
	//		ss.str("");
	//		ss << R"(#version 330 core
	//layout(location = 0) in vec3 aPos;
	//layout(location = 1) in vec3 aNormal;
	//layout(location = 2) in vec2 aUV;
	//layout(location = 3) in mat4 Transform;
	//
	//uniform mat4 projection;
	//uniform mat4 view;)"
	//<<
	//ShadowFVar()
	//<<
	//R"(void main()
	//{
	//UV = vec2(aUV.x, aUV.y);
	//vec4 worldPos = Transform * vec4(aPos, 1.0);
	//FragPos = worldPos.xyz;
	//
	//mat3 normalMatrix = transpose(inverse(mat3(Transform)));
	//Normal = normalMatrix * aNormal;
	//
	//gl_Position = projection * view * worldPos;)"
	//<<
	//ShadowFMain()
	//<<
	//"}\n\0";
	//		return ss.str();
	//	}
	//	inline std::string DepthFragment() override
	//	{
	//		ss.str("");
	//		ss <<
	//			"#version 330 core\n"
	//			"layout(location = 0) out vec3 gPosition;\n"
	//			"layout(location = 1) out vec3 gNormal;\n"
	//			"in vec2 UV;\n"
	//			"in vec3 FragPos;\n"
	//			"in vec3 Normal;\n"
	//			"out vec4 FragColor;\n"
	//			<<
	//			FVar()
	//			<<
	//			" void main()\n"
	//			" {\n"
	//			"   gPosition = FragPos;\n"
	//			"   // also store the per-fragment normals into the gbuffer\n"
	//			"   gNormal = normalize(Normal);\n"
	//			<<
	//			FMain()
	//			<<
	//			" }\n\0";
	//		return ss.str();
	//	}
		//[0] [0] = 0;
};
class Shader_CN :public Shader3d
{
public:
	inline void init()override
	{

	};
protected:
	inline std::string VVar() override
	{
		return
			R"(
layout (location = 2) in vec2 aUV;
out vec2 UV;)";
	}
	inline std::string VMain() override
	{
		return "UV= aUV;";

	}
	inline std::string FVar() override
	{
		return
			R"(
in vec2 UV;
//uniform sampler2D specular;
uniform sampler2D albedo;
uniform vec2 UVScale;
)";
	}
	inline std::string FMain() override
	{
		return
			"FragColor =texture(albedo, UV*UVScale);\n";
	}

	//const char* GetVertexCode() override
	//{
	//    return"#version 330 core\n"
	//        "layout(location = 0) in vec3 aPos;\n"
	//        "layout(location = 1) in vec2 aTexCoord;\n"
	//        " out vec2 TexCoord;\n"
	//        " void main()\n"
	//        " {\n"
	//        "     gl_Position = vec4(aPos, 1.0);\n"
	//        "     TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
	//        " }\n\0";
	//}
	//const char* GetFragmentCode() override
	//{
	//    return"#version 330 core\n"
	//        "out vec4 FragColor;\n"
	//        "in vec2 TexCoord;\n"
	//        "// texture samplers\n"
	//        "uniform sampler2D texture1;\n"
	//        "void main()\n"
	//        "{\n"
	//        "  // linearly interpolate between both textures (80% container, 20% awesomeface)\n"
	//        "  FragColor = texture(texture1, TexCoord);\n"
	//        "}\n\0";
	//}

//	inline std::string GetVertexCode() override
//	{
//		ss.str("");
//		ss << R"(#version 330 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aUV;
//layout(location = 3) in mat4 Transform;
//
//uniform mat4 projection;
//uniform mat4 view;
//out vec3 FragPos;  
//out vec2 UV;
//out vec3 Normal;   
//out vec2 UV;)"
//<<
//VVar()
//<<
//R"(void main()
//{
//UV = vec2(aUV.x, aUV.y);
//vec4 worldPos = Transform * vec4(aPos, 1.0);
//FragPos = worldPos.xyz;
//
//mat3 normalMatrix = transpose(inverse(mat3(Transform)));
//Normal = normalMatrix * aNormal;
//
//gl_Position = projection * view * worldPos;)"
//<<
//VMain()
//<<
//"}\n\0";
//		return ss.str();
//	}
//	inline std::string GetFragmentCode() override
//	{
//		ss.str("");
//		ss <<
//			"#version 330 core\n"
//			"layout(location = 0) out vec3 gPosition;\n"
//			"layout(location = 1) out vec3 gNormal;\n"
//			"in vec2 UV;\n"
//			"in vec3 FragPos;\n"
//			"in vec3 Normal;\n"
//			"out vec4 FragColor;\n"
//			<<
//			FVar()
//			<<
//			" void main()\n"
//			" {\n"
//			"   gPosition = FragPos;\n"
//			"   // also store the per-fragment normals into the gbuffer\n"
//			"   gNormal = normalize(Normal);\n"
//			<<
//			FMain()
//			<<
//			" }\n\0";
//		return ss.str();
//	}
};
class ScreenSpaceShadowShader :public Shader
{
	inline std::string GetVertexCode() override
	{
		return std::format(R"({}
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out  vec2 UV;
void main()
{{
UV = aTexCoords;
gl_Position = vec4(aPos, 1.0);
}})",

OpenGlShaderVersion);
	}
	inline std::string GetFragmentCode() override
	{
		return std::format(R"({}
out vec4 FragColor;

    in vec2 UV;

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform mat4 inv_view_proj_matrix;
uniform mat4 inv_lightSpaceMatrix;
uniform mat4 lightProjMatrix;
uniform mat4 lightViewMatrix;
uniform sampler2D shadowMap;
uniform sampler2D cameraDepth;
uniform float shadowPrecision;
uniform vec3 lightPos;
uniform vec3 viewPos;
const float ts = 1/2048;
const float texelsizeH = 1/800;
const float texelsizeW = 1/600;

uint hash( uint x ) {{
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}}
in flat int InstanceID ;


// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) {{ return hash( v.x ^ hash(v.y)                         ); }}
uint hash( uvec3 v ) {{ return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }}
uint hash( uvec4 v ) {{ return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }}



// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {{
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}}



// Pseudo-random value in half-open range [0:1].
float random( float x ) {{ return floatConstruct(hash(floatBitsToUint(x))); }}
float random( vec2  v ) {{ return floatConstruct(hash(floatBitsToUint(v))); }}
float random( vec3  v ) {{ return floatConstruct(hash(floatBitsToUint(v))); }}
float random( vec4  v ) {{ return floatConstruct(hash(floatBitsToUint(v))); }}
float distS( vec3 A, vec3 B )
{{

    vec3 C = A - B;
    return dot( C, C );

}}
//vec3 WorldSpaceFromDepth(float depth,vec2 uv,mat4 inv_matrix)
//{{    
//    depth = depth * 2.0 - 1.0;
//	vec4 v2 = inv_matrix*vec4(uv,depth,1);
//    return v2.xyz/v2.w;
//}}
vec3 WorldSpaceFromDepth(float depth,vec2 uv,mat4 projMatrixInv,mat4 viewMatrixInv) {{
	projMatrixInv = inverse(projMatrixInv);
	viewMatrixInv = inverse(viewMatrixInv);

    depth = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, depth, 1.0);
    vec4 viewSpacePosition = projMatrixInv * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = viewMatrixInv * viewSpacePosition;

    return worldSpacePosition.xyz;
}}
vec2 ndcspace(mat4 projectionMatrix,mat4 viewMatrix,vec3 position)
{{
	vec4 clipSpacePos = projectionMatrix * (viewMatrix * vec4(position, 1.0));
	vec3 ndcSpacePos = clipSpacePos.xyz / clipSpacePos.w;
	//vec2 screenSpace = ((ndcSpacePos.xy + 1.0) / 2.0) * vec2(800,600);
	//screenSpace.y = 600 - screenSpace.y;
	return (ndcSpacePos.xy+ 1.0) / 2.0;
}}
//float smoth(sampler2D tex,vec2 uv)
//{{
//	vec2 grid= vec2(texelsize/2);
//	vec2 cent = uv - mod(uv,texelsize)+grid;
//	float src = texture(tex,cent).r;
//	float res = distamce(cent,uv)*src;
//	
//	if(grid.x>=0)
//	{{	
//		cent+vec2(grid.x,0);
//		src = texture(tex,cent).r;
//		res = distamce(cent,uv)*src;
//	}}
//	float srcl = texture
//}}
vec4 cubic(float v){{
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w) * (1.0/6.0);
}}
float textureBicubic(sampler2D sampler, vec2 texCoords){{

   vec2 texSize = textureSize(sampler, 0);
   vec2 invTexSize = 1.0 / texSize;
   
   texCoords = texCoords * texSize - 0.5;

   
    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    vec4 c = texCoords.xxyy + vec2 (-0.5, +1.5).xyxy;
    
    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4 (xcubic.yw, ycubic.yw) / s;
    
    offset *= invTexSize.xxyy;
    
    float sample0 = texture(sampler, offset.xz).r;
    float sample1 = texture(sampler, offset.yz).r;
    float sample2 = texture(sampler, offset.xw).r;
    float sample3 = texture(sampler, offset.yw).r;

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(
       mix(sample3, sample2, sx), mix(sample1, sample0, sx)
    , sy);
}}

float getSmuthDepth(sampler2D t, vec2 uv)
{{
	float shadow=0.0;
    //vec2 texelSize = vec2(1.0 / textureSize(t, 0).x);

	// shadow +=texture(t,  uv).r;        
	// shadow +=texture(t,  uv + vec2(ts	, 0))   .r;        
	// shadow +=texture(t,  uv - vec2(ts	, 0))   .r;        
	// shadow +=texture(t,  uv + vec2(0	, ts))  .r;        
	// shadow +=texture(t,  uv - vec2(0	, ts))  .r;        
	// shadow +=texture(t,  uv + vec2(ts	, ts))  .r;        
	// shadow +=texture(t,  uv + vec2(-ts	, ts))  .r;        
	// shadow +=texture(t,  uv - vec2(ts	, ts))  .r;        
	// shadow +=texture(t,  uv - vec2(-ts	, ts))  .r;        
 //   shadow /= 9.0;
	//return shadow;

	float s=0;
	int sampleC=2;

    for(int x = -sampleC; x <= sampleC; ++x)
    {{
        for(int y = -sampleC; y <= sampleC; ++y)
        {{
            shadow +=textureBicubic(t,  uv + vec2(x, y)*ts).r;
			s++;        
        }}    
    }}
    shadow /= s;
	return shadow;
}}

float ShadowCalculation()
{{
	float dth=texture(cameraDepth,UV).r;
	if(dth>0.99996)return 0;
	//float dth=texture(cameraDepth,UV).r;
    vec4 worldPos =lightProjMatrix *lightViewMatrix* vec4(WorldSpaceFromDepth(dth,UV,proj_matrix, view_matrix),1.0);

	vec3 projCoords = worldPos.xyz / worldPos.w;
    projCoords  = projCoords  * 0.5 + 0.5;

    float currentDepth =projCoords.z;
    //float closestDepth = getSmuthDepth(shadowMap, projCoords.xy); 
    //float shadow = currentDepth  >= closestDepth  ?  0.84:0.0 ;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x)
	{{
		for(int y = -2; y <= 2; ++y)
		{{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth-0.00000004> pcfDepth ? 0.35 : 0.0;        
		}}    
	}}
	shadow /= 16.0*dth;

    return shadow;
    //vec2 LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
    //vec3 worldPos2= WorldSpaceFromDepth(getSmuthDepth(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix);
	//
	//float texelSize;

   //float sumDst=distance(worldPos2,worldPos);



	
//worldPos = WorldSpaceFromDepth(textureBicubic(cameraDepth,UV+vec2(texelsizeH,texelsizeW)),UV+vec2(texelsizeH,texelsizeW),proj_matrix, view_matrix);
//	LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
//    worldPos2= WorldSpaceFromDepth(textureBicubic(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix);
//sumDst+=distance(worldPos2,worldPos);
//
//worldPos = WorldSpaceFromDepth(textureBicubic(cameraDepth,UV-vec2(texelsizeH,texelsizeW)),UV-vec2(texelsizeH,texelsizeW),proj_matrix, view_matrix);
//    LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
//    worldPos2= WorldSpaceFromDepth(textureBicubic(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix);
//sumDst+=distance(worldPos2,worldPos);
//
//worldPos = WorldSpaceFromDepth(textureBicubic(cameraDepth,UV-vec2(0,texelsizeW)),UV-vec2(0,texelsizeW),proj_matrix, view_matrix);
//    LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
//    worldPos2= WorldSpaceFromDepth(textureBicubic(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix);
//sumDst+=distance(worldPos2,worldPos);
//
//worldPos = WorldSpaceFromDepth(textureBicubic(cameraDepth,UV-vec2(texelsizeH,0)),UV-vec2(texelsizeH,0),proj_matrix, view_matrix);
//    LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
//    worldPos2= WorldSpaceFromDepth(textureBicubic(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix);
//sumDst+=distance(worldPos2,worldPos);
//
//worldPos = WorldSpaceFromDepth(textureBicubic(cameraDepth,UV+vec2(texelsizeH,0)),UV+vec2(texelsizeH,0),proj_matrix, view_matrix);
//    LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
//    worldPos2= WorldSpaceFromDepth(textureBicubic(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix); 
//sumDst+=distance(worldPos2,worldPos);  
// 
//worldPos = WorldSpaceFromDepth(textureBicubic(cameraDepth,UV+vec2(0,texelsizeW)),UV+vec2(0,texelsizeW),proj_matrix, view_matrix);
//	LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
//    worldPos2= WorldSpaceFromDepth(textureBicubic(shadowMap, LightUV),LightUV,lightProjMatrix,lightViewMatrix);
//sumDst+=distance(worldPos2,worldPos);

//float dst= distance(worldPos2,worldPos);


//dst = sumDst/7;

//return dst-0.05;


  /*  return (dst<=0.0002?  0.0: 0.6);*/
 /* return max(1-(1/sqrt(1+dst*25)),0.0);*/
	//return distance(worldPos2,worldPos)>1212?  0.0: 0.6;
    //return distance(worldPos2,worldPos)/8800  /*?  0.0: 0.6*/ ;

}}


void main()
{{           
 //   vec3 worldPos = WorldSpaceFromDepth(texture(cameraDepth,UV).r,UV,proj_matrix, view_matrix);
 //   vec2 LightUV  = ndcspace(lightProjMatrix,lightViewMatrix,worldPos);
 //   vec3 worldPos2= WorldSpaceFromDepth(texture(shadowMap, LightUV).r,LightUV,lightProjMatrix,lightViewMatrix);
	//FragColor = vec4(worldPos2,0.9);
	//FragColor = vec4(LightUV,0.1,0.9);
	//ShadowCalculation()         

         float c =ShadowCalculation();
		if(c<0.5)
    FragColor = vec4(0.02,0.04 ,0.01,c*(random(UV*c)/1.5+0.75));
		else 
	FragColor = vec4(0.02,0.04 ,0.01,c);

//FragColor = vec4(WorldSpaceFromDepth(texture(cameraDepth,UV).r,UV, inv_view_proj_matrix),0.8);
  //FragColor = vec4(ShadowCalculation(),1);
	//FragColor = vec4(WorldSpaceFromDepth(texture(cameraDepth,UV).r,UV, inv_view_proj_matrix),1);
}})",

OpenGlShaderVersion);
	}
};

static class DeffaultShader
{
public:
	static std::optional < Shader> MissingShader;
	inline static void init()
	{
		MissingShader = Shader();
	}
};
std::optional < Shader> DeffaultShader::MissingShader;

#pragma endregion

#pragma region Material


class Material
{
public:
	//Dictionary <string, int> uniformsMap;
	Dictionary <string, Texture2d*> textures;
	Dictionary <string, glm::vec2> Vect2s;
	Dictionary <string, GLfloat> GLfloats;
	Dictionary <string, int> ints;
	Shader* shader;
public:
	Material()
	{
		shader = &DeffaultShader::MissingShader.value();
	}
	Material(Shader* s) { shader = s; };
	//inline void init() 
	//{
	//	if(!shader->IsUniformsInited)
	//	{ 
	//		shader->IsUniformsInited = true;
	//		GLuint t = 0;
	//		shader->use();
	//		for (const auto& [key, value] : textures.data) {
	//			shader->Uniforms[key] = glGetUniformLocation(shader->MainId, key.c_str());
	//			glUniform1i(shader->Uniforms[key], t);
	//			t++;
	//		}
	//	}
	//}
	virtual void UseMaterial() { shader->use(); }
	void UseTextures()
	{
		GLuint t = 0;
		for (const auto& [key, value] : textures.data) {
			glActiveTexture(GL_TEXTURE0 + t);
			glUniform1i(glGetUniformLocation(shader->MainId, key.c_str()), t);
			glBindTexture(GL_TEXTURE_2D, value->Id);
			t++;
			/*	std::cout << "Puck" << std::endl;*/
		}

	}
	void UseVect2s()
	{
		GLuint t = 0;
		for (const auto& [key, value] : Vect2s.data) {
			glUniform2fv(glGetUniformLocation(shader->MainId, key.c_str()), 1, &value[0]);
		}

	}

#pragma region Shader_SetFuncs


	inline  void setBool(const char name[], int value)
	{
		if (ints.contains(name))
		{
			ints[name] = value;
			glUniform1i(glGetUniformLocation(shader->MainId, name), value);

		}
		else
		{
#ifdef Debug 
			SetColor(LightRed, Black);
			std::cout << "Shader (" << typeid(*this).name() << ")setBool(int)-> " << ShaderParamError << "(" << name << " = " << value << ")\n";
			SetColor(White, Black);
#endif
		}
	}
	//------------------------------------------------------------------------
	inline void setBool(const char name[], bool value)
	{
		glUniform1i(glGetUniformLocation(shader->MainId, name), (int)value);
	}
	// ------------------------------------------------------------------------
	inline void setTexture(const char name[], Texture2d* value)
	{
		if (textures.contains(name))
		{
			textures[name] = value;
			glUniform1i(glGetUniformLocation(shader->MainId, name), value->Id);
		}
		else
		{
#ifdef Debug 
			SetColor(LightRed, Black);
			std::cout << "Shader (" << typeid(*this).name() << ")setTexture-> " << ShaderParamError << "(" << name << " = " << value->Id << " - Texture Id)\n";
			SetColor(White, Black);
#endif
		}
	}
	inline void setInt(const char name[], int value)
	{

		if (ints.contains(name))
		{
			ints[name] = value;
			glUniform1i(glGetUniformLocation(shader->MainId, name), value);
		}
		else
		{
#ifdef Debug 
			SetColor(LightRed, Black);
			std::cout << "Shader (" << typeid(*this).name() << ")setInt-> " << ShaderParamError << "(" << name << " = " << value << ")\n";
			SetColor(White, Black);
#endif
		}
	}
	inline void  dynamicSetInt(const char name[], int value)
	{
		glUniform1i(glGetUniformLocation(shader->MainId, name), value);
	}
	// ------------------------------------------------------------------------
	inline void dynamicSetFloat(const char name[], GLfloat value)
	{
		glUniform1f(glGetUniformLocation(shader->MainId, name), value);
	}
	inline void setFloat(const char name[], GLfloat value)
	{
		if (GLfloats.contains(name))
		{
			GLfloats[name] = value;
			glUniform1f(glGetUniformLocation(shader->MainId, name), value);
		}
		else
		{
#ifdef Debug 
			SetColor(LightRed, Black);
			std::cout << "Shader (" << typeid(*this).name() << ")setFloat-> try to set unexisting shader param(" << name << " = " << value << ")\n";
			SetColor(White, Black);
#endif
		}

	}
	inline void setVec2(const char name[], const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(shader->MainId, name), 1, &value[0]);
	}
	inline void setVec2(const char name[], GLfloat x, GLfloat y)
	{
		glUniform2f(glGetUniformLocation(shader->MainId, name), x, y);
	}
	// ------------------------------------------------------------------------
	inline void setVec3(const char name[], const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(shader->MainId, name), 1, &value[0]);
	}
	inline void setVec3(const char name[], GLfloat x, GLfloat y, GLfloat z)
	{
		glUniform3f(glGetUniformLocation(shader->MainId, name), x, y, z);
	}
	// ------------------------------------------------------------------------
	inline void setVec4(const char name[], const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(shader->MainId, name), 1, &value[0]);
	}
	inline void setVec4(const char name[], GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(shader->MainId, name), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	inline void setMat2(const char name[], const glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(shader->MainId, name), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	inline void setMat3(const char name[], const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(shader->MainId, name), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	inline void setMat4(const char name[], const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader->MainId, name), 1, GL_FALSE, &mat[0][0]);
	}

#pragma endregion
};

class Material_CNShader :public Material
{
public:
	Material_CNShader(Shader* st, Texture2d* albedo, Texture2d* specular, const glm::vec2& UVScale)
	{
		shader = st;
		textures["albedo"] = albedo;
		textures["specular"] = specular;
		Vect2s["UVScale"] = UVScale;
		//init();
	}
	void UseMaterial()override
	{
		shader->use();
		UseTextures();
		UseVect2s();
	}
};


#pragma endregion

#pragma region Mesh
class Mesh
{

public:
	GLuint VAO, VBO, EBO;
	std::string name;
	//vector<Vertex*> vertices;
	//vector<GLuint*> triangles;
	vector<Vertex> vertices;
	vector<GLuint> triangles;
	vector <unsigned int> subMeshes;
	vector <std::string> subNames;
	glm::vec3 pivot;
	glm::vec3 bouds;
public:

	Mesh() { name = "Unnamed";  }
	Mesh(const char* Name) { name = Name;}
	bool LoadFromFile(const char* filename)
	{
		std::ifstream in(AppPath+ "\\" + filename, std::ios::in);
		if (!in)
		{
			std::cerr << "Cannot open " << AppPath + "\\" + filename << std::endl;
		}
		std::string line;
		vector <glm::vec3> verts;
		vector <glm::vec2> uv;
		int i=0;
		while (std::getline(in, line))
		{
			//check v for vertices
			if (line.substr(0, 2) == "v ") {
				std::istringstream v(line.substr(2));
				glm::vec3 vert;
				float x, y, z;
				v >> x; v >> y; v >> z;
				vert = glm::vec3(x, y, z);
				verts.push_back(vert);
			}
			//check for texture co-ordinate
			else if (line.substr(0, 2) == "vt") {

				std::istringstream v(line.substr(3));
				glm::vec2 tex(0.0f);
				v >> tex.x >> tex.y;
				tex.y = 1.0f - tex.y;
				uv.push_back(tex);

			}
			//check for faces
			else if (line.substr(0, 2) == "f ") {
				int a, b, c; //to store mesh index
				int A, B, C; //to store texture index
				//std::istringstream v;
			  //v.str(line.substr(2));
				const char* chh = line.c_str();
				sscanf_s(chh, "f %i/%i %i/%i %i/%i", &a, &A, &b, &B, &c, &C); //here it read the line start with f and store the corresponding values in the variables

				//v>>a;v>>b;v>>c;
				a--; b--; c--;
				A--; B--; C--;
				//std::cout<<a<<b<<c<<A<<B<<C;
				vertices.push_back(Vertex(verts[c], glm::vec3(1), uv[C]));
				vertices.push_back(Vertex(verts[b], glm::vec3(1), uv[B]));
				vertices.push_back(Vertex(verts[a], glm::vec3(1), uv[A]));
			
				triangles.push_back(i); triangles.push_back(i+1);  triangles.push_back(i+2);
				i += 3;
			}
			else if (line.substr(0, 3) == "use")
			{
				std::istringstream v(line.substr(6));
				string s;
				v >> s;
				subNames.push_back(s);
				subMeshes.push_back(triangles.size());
			}
			else if (line.substr(0, 2) == "o ")
			{
				std::istringstream v(line.substr(2));
				v >> name;
			}
		}
		in.close();
		CalcAll();
		return true;
	}
	void cleartemp()
	{
		vertices.~vector();
		triangles.~vector();
	}
	void CalcAll()
	{
		CalculateBoundings();
		CalculatePivot();
	}
	void CalculateBoundings()
	{
		float mx = 0;
		float my = 0;
		float mz = 0;
		float l = 0;
		glm::vec3 abs = glm::abs(pivot);
		for (const Vertex&  v : vertices)
		{
			if ((glm::abs(v.Position.x) - abs.x) * 2 > mx)mx = (glm::abs(v.Position.x) - abs.x) * 2;
			if ((glm::abs(v.Position.y) - abs.y) * 2 > my)my = (glm::abs(v.Position.y) - abs.y) * 2;
			if ((glm::abs(v.Position.z) - abs.z) * 2 > mz)mz = (glm::abs(v.Position.z) - abs.z) * 2;
		}
		bouds = glm::vec3(mx, my, mz);
	}
	void CalculatePivot()
	{
		if (vertices.size() <= 0)return;
		pivot = vertices[0].Position;
		for (const Vertex& v : vertices)
		{
			pivot += v.Position ;
		}
		pivot /= vertices.size();
	}
	//void LoadGLBuffers()
	//{
	//    glGenVertexArrays(1, &VAO);
	//    glGenBuffers(1, &VBO);
	//    glGenBuffers(1, &EBO);

	//    glBindVertexArray(VAO);
	//    glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int),
	//        &triangles[0], GL_STATIC_DRAW);

	//    // vertex positions
	//    glEnableVertexAttribArray(0);
	//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//    // vertex normals
	//    glEnableVertexAttribArray(1);
	//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//    // vertex texture coords
	//    glEnableVertexAttribArray(2);
	//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

	//    glBindVertexArray(0);
	//}
	//void BindVAO()
	//{
	//    glBindVertexArray(VAO);
	//}
	//inline void Draw(Material* m)
	//{
	//    m->UseMaterial();
	//    // draw mesh
	//  /*  glBindVertexArray(VAO);*/
	//    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
	//   
	//}
};
#pragma region DEFFAUT__MESH
class Mesh_Premetive :public Mesh
{
public:
	Mesh_Premetive(const char* Name) :Mesh(Name) { subMeshes.push_back(0u); };
	Mesh_Premetive() :Mesh("Premetive") { subMeshes.push_back(0u); };
};
// V: 24, T:12*3
class M_Cube :public Mesh_Premetive
{
private:
	//    static constexpr GLuint tris[] = {
	//        //Top
	//        2, 6, 7,
	//            2, 3, 7,

	//            //Bottom
	//            0, 4, 5,
	//            0, 1, 5,

	//            //Left
	//            0, 2, 6,
	//            0, 4, 6,

	//            //Right
	//            1, 3, 7,
	//            1, 5, 7,

	//            //Front
	//            0, 2, 3,
	//            0, 1, 3,

	//            //Back
	//            4, 6, 7,
	//            4, 5, 7
	//    };
	//    static constexpr GLfloat vert[]= {
	//    -1, -1,  1, //0 Right Bot Frnt
	//     1, -1,  1, //1 Left  Bot Frnt
	//    -1,  1,  1, //2 Right Top Frnt
	//     1,  1,  1, //3 Left  Top Frnt
	//    -1, -1, -1, //4 Right Bot Back
	//     1, -1, -1, //5 Left  Bot Back
	//    -1,  1, -1, //6 Right Top Back 
	//     1,  1, -1  //7 Left  Top Back 
	//};
	//     //    *---*     0
	//     //    |   |
	//     //*---*---*---* 0.25
	//     //|   |   |   |
	//     //*---*---*---* 0.5 
	//     //    |   |
	//     //    *---*     0.75
	//     //    |   |
	//     //    *---*     1
	//     //0  0.33 0.66 1
	//    static constexpr GLfloat uvs[] = {
	//    0.66f, 1   , //0 Right Bot Frnt     
	//    0.33f, 1   , //1 Left  Bot Frnt 
	//    0.66f,0.25f, //2 Right Top Frnt     
	//    0.33f,0.25f, //3 Left  Top Frnt 
	//    0.66f,0.75f, //4 Right Bot Back 
	//    0.33f,0.75f, //5 Left  Bot Back 
	//    0.66f,0.5f , //6 Right Top Back     
	//    0.33f,0.5f   //7 Left  Top Back 
	//    };  

		//Vector3[] vertices = {
		//    new Vector3(0, size, 0)  ,
		//    new Vector3(0, 0, 0      ),
		//    new Vector3(size, size, 0),
		//    new Vector3(size, 0, 0   ),

		//    new Vector3(0, 0, size        ),
		//    new Vector3(size, 0, size     ),
		//    new Vector3(0, size, size   ),
		//    new Vector3(size, size, size),

		//    new Vector3(0, size, 0),
		//    new Vector3(size, size, 0),

		//    new Vector3(0, size, 0),
		//    new Vector3(0, size, size),

		//    new Vector3(size, size, 0),
		//    new Vector3(size, size, size),
		//};

		//int[] triangles = {
		//    0, 2, 1, // front
		//    1, 2, 3,
		//    4, 5, 6, // back
		//    5, 7, 6,
		//    6, 7, 8, //top
		//    7, 9 ,8,
		//    1, 3, 4, //bottom
		//    3, 5, 4,
		//    1, 11,10,// left
		//    1, 4, 11,
		//    3, 12, 5,//right
		//    5, 12, 13


		//};


		//Vector2[] uvs = {
		//    new Vector2(0    , 0.66f,
		//    new Vector2(0.25f, 0.66f,
		//    new Vector2(0    , 0.33f,
		//    new Vector2(0.25f, 0.33f,

		//    new Vector2(0.5f , 0.66f,
		//    new Vector2(0.5f , 0.33f,
		//    new Vector2(0.75f, 0.66f,
		//    new Vector2(0.75f, 0.33f,

		//    new Vector2(1    , 0.66f,
		//    new Vector2(1    , 0.33f,

		//    new Vector2(0.25f, 1,
		//    new Vector2(0.5f , 1,

		//    new Vector2(0.25f, 0,
		//    new Vector2(0.5f , 0,
		//};
//1, 2, 3,
//3, 1, 0,
////Top
//4, 5, 6,
//5, 6, 7,
////Bottom
//8, 9, 10,
//9, 10, 11,
////Lelt
//12, 13, 14,
//13, 14, 15,
////right
//16, 17, 18,
//17, 18, 19,
////front
//20, 21, 22,
//21, 22, 23
////back
	static constexpr GLuint tris[] = {
		0,1,2,
		3,2,1,
		//Top
		6,5,4,
		5,6,7,
		//Bottom
		8,9,10,
		11,10,9,
		//Lelt
		14,13,12,
		13,14,15,
		//right
		18,17,16,
		17,18,19,
		//front
		20,21,22,
		23,22,21
		//back
	};
	static constexpr GLfloat vert[] =
	{
		-0.5f, 0.5f, 0.5f, //R U    0
		-0.5f, 0.5f,-0.5f,//R B     1
		 0.5f, 0.5f, 0.5f,  //L U   2
		 0.5f, 0.5f,-0.5f, //L B    3
		 //Top
		-0.5f,-0.5f, 0.5f, //R U    4
		-0.5f,-0.5f,-0.5f,//R B     5
		 0.5f,-0.5f, 0.5f,  //L U   6
		 0.5f,-0.5f,-0.5f, //L B    7
		 //Down
		-0.5f, 0.5f, 0.5f, //R U    8
		-0.5f,-0.5f, 0.5f,//R B     9
		-0.5f , 0.5f,-0.5f,  //L U   10
		-0.5f ,-0.5f,-0.5f, //L B    11
		//Left
		 0.5f, 0.5f, 0.5f, //R U    12
		 0.5f,-0.5f, 0.5f,//R B     13
		 0.5f , 0.5f,-0.5f,  //L U  14
		 0.5f ,-0.5f,-0.5f, //L B   15
		 //right
		-0.5f, 0.5f, 0.5f, //R U    20
		-0.5f,-0.5f, 0.5f,//R B     21
		 0.5f, 0.5f, 0.5f,  //L U   22
		 0.5f,-0.5f, 0.5f, //L B    23
		 // Front
		 -0.5f, 0.5f,-0.5f, //R U    16
		 -0.5f,-0.5f,-0.5f,//R B     17
		 0.5f , 0.5f,-0.5f,  //L U   18
		 0.5f ,-0.5f,-0.5f, //L B    19
		 //Back



	};
	static constexpr GLfloat uvs[] = {
		0,0,
		1,0,
		0,1,
		1,1,
	};
	//    *---*     0
	//    |   |
	//*---*---*---* 0.25
	//|   |   |   |
	//*---*---*---* 0.5 
	//    |   |
	//    *---*     0.75
	//    |   |
	//    *---*     1
	//0  0.33 0.66 1

public:
	M_Cube(GLfloat Size = 1) :Mesh_Premetive("Cube")
	{
		int s = sizeof(vert) / sizeof(*vert);
		for (GLint i = 0; i < s; i += 12)
		{
			vertices.push_back(Vertex(glm::vec3(vert[i], vert[i + 1], vert[i + 2]) * Size, glm::vec3(tris[i], tris[i + 1], tris[i + 2]), glm::vec2(uvs[0], uvs[1])));
			vertices.push_back(Vertex(glm::vec3(vert[i + 3], vert[i + 4], vert[i + 5]) * Size, glm::vec3(tris[i], tris[i + 1], tris[i + 2]), glm::vec2(uvs[2], uvs[3])));
			vertices.push_back(Vertex(glm::vec3(vert[i + 6], vert[i + 7], vert[i + 8]) * Size, glm::vec3(tris[i], tris[i + 1], tris[i + 2]), glm::vec2(uvs[4], uvs[5])));
			vertices.push_back(Vertex(glm::vec3(vert[i + 9], vert[i + 10], vert[i + 11]) * Size, glm::vec3(tris[i], tris[i + 1], tris[i + 2]), glm::vec2(uvs[6], uvs[7])));
		}
		triangles = vector<GLuint>(tris, tris + sizeof tris / sizeof tris[0]);
		CalcAll();
	}
};
class M_Sphere :public Mesh_Premetive
{
public:
	M_Sphere() {};
	M_Sphere(GLfloat radius, GLuint rings, GLuint sectors) :Mesh_Premetive("Sphere")
	{
		GLfloat const R = 1. / (GLfloat)(rings - 1);
		GLfloat const S = 1. / (GLfloat)(sectors - 1);
		int r, s;

		//vertices.reserve(rings * sectors*sizeof(Vertex));
		//triangles.reserve(rings * sectors * 4);
		GLuint i = 0;

		for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
			GLfloat const y = sin(-Pi2 + Pi * r * R);
			GLfloat const x = cos(2 * Pi * s * S) * sin(Pi * r * R);
			GLfloat const z = sin(2 * Pi * s * S) * sin(Pi * r * R);

			//vertices[i]=Vertex(glm::vec3(x* radius, y* radius, z* radius), glm::vec3(x, y, z), glm::vec2(s* S, r* R));

			//triangles[i]=r * sectors + s;
			//triangles[i+1] = r * sectors + (s + 1);
			//triangles[i + 2] = (r + 1) * sectors + (s + 1);
			//triangles[i + 3] = (r + 1) * sectors + s;
			vertices.push_back(Vertex(glm::vec3(x * radius, y * radius, z * radius), glm::vec3(x, y, z), glm::vec2(s * S, r * R)));

			triangles.push_back(r * sectors + s);
			triangles.push_back(r * sectors + (s + 1));
			triangles.push_back((r + 1) * sectors + (s + 1));
			triangles.push_back((r + 1) * sectors + (s + 1));
			triangles.push_back((r + 1) * sectors + s);
			triangles.push_back(r * sectors + s);

		}
		CalcAll();
	}
	void Resize(GLfloat newsize)
	{
		GLuint count = vertices.size();
		for (GLuint i = 0; i < count; i++)
		{
			vertices[i].Position = vertices[i].Normal * newsize;
		}
		CalcAll();
	}
};
class M_NoisePlane :public Mesh_Premetive
{
public:
	M_NoisePlane(float XSize, float YSize, float freq)
	{
		float dx = 0;
		float dy = 0;
		glm::vec3 A;
		glm::vec3 B;
		Noise n = Noise(162678);

		for (float y = 0; y < YSize; y++)
		{
			for (float x = 0; x < XSize; x++)
			{
				dx = x / XSize;
				dy = y / YSize;
				vertices.push_back(Vertex(glm::vec3(x, n.Get2D(dx * freq, dy * freq, 4) * 2, y), vec3Up, glm::vec2(dx, dy)));
			}
		}
		const int xs = (XSize - 1);
		const int ys = (YSize - 1);
		int t = 0;
		for (float y = 0; y < ys; y++)
		{
			for (float x = 0; x < xs; x++)
			{
				triangles.push_back(t);
				triangles.push_back(t + 1);
				triangles.push_back(t + XSize);

				triangles.push_back(t + XSize + 1);
				triangles.push_back(t + XSize);
				triangles.push_back(t + 1);
				t++;

				A = vertices[t + 1].Position - vertices[t].Position;
				B = vertices[t + XSize].Position - vertices[t].Position;
				B = glm::vec3(A.x * B.z - A.z * A.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
				vertices[t].Normal = B;
				vertices[t + 1].Normal = B;
				vertices[t + XSize].Normal = B;
				A = vertices[t + 1].Position - vertices[t + XSize].Position;
				B = vertices[t + XSize + 1].Position - vertices[t + XSize].Position;
				B = glm::vec3(A.x * B.z - A.z * A.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
				vertices[t + 1].Normal = B;
				vertices[t + XSize].Normal = B;
				vertices[t + XSize + 1].Normal = B;
			}
			t++;
		}
	}
};
#pragma endregion
#pragma endregion

#pragma region Objects
class GameObject
{
public:
	string name;
	Transform transform;
	GameObject(string Name, glm::vec3 Position)
	{
		name = Name;
		transform.position = Position;
		transform.scale = glm::vec3(1);
	}
	GameObject(string Name)
	{
		name = Name;
		transform.position = glm::vec3(0);
		transform.scale = glm::vec3(1);
	}
	GameObject()
	{
		name = "GameObject";
		transform.position = glm::vec3(0);
		transform.scale = glm::vec3(1);
	}
};
class RenderedObject :public GameObject
{
public:
	Mesh* mesh;
	vector<Material*>/*_Shader_Test*/ material;

public:
	RenderedObject(Mesh& m, Material/*_Shader_Test*/& mat) :GameObject(m.name)
	{
		mesh = &m;
		material.push_back(& mat);
	}
	RenderedObject(Mesh& m, Material/*_Shader_Test*/& mat, string Name) :GameObject(Name)
	{
		mesh = &m;
		material.push_back(&mat);
	}
	RenderedObject(Mesh& m, Material/*_Shader_Test*/& mat, glm::vec3 Position) :GameObject(m.name, Position)
	{
		mesh = &m;
		material.push_back(&mat);
	}
	RenderedObject(Mesh& m, std::vector<Material*> mat) :GameObject(m.name)
	{
		mesh = &m;
		material = mat;
	}
	RenderedObject(Mesh& m, std::vector<Material*> mat, string Name) :GameObject(Name)
	{
		mesh = &m;
		material = mat;
	}
	RenderedObject(Mesh& m, std::vector<Material*> mat, glm::vec3 Position) :GameObject(m.name, Position)
	{
		mesh = &m;
		material = mat;
	}
};
#pragma endregion

#pragma region Renders

class NamedObject {
public:
	string Name;
};
class Camera : NamedObject
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::mat4 projection;
	glm::mat4 viewMatrix;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
public:
	Camera() {};
	// constructor with vectors
	Camera(glm::mat4 project,string name,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0,
		float pitch = 0) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(2),
		MouseSensitivity(0.2),
		Zoom(75)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		projection = project;
		Name = name;
		updateCameraVectors();
	}
public:
	void LookAt(glm::vec3 pos)
	{
		Front = normalize(pos - Position);
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}


	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
public:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
		viewMatrix = GetViewMatrix();
	}
};


Array<Camera> allCameras;
int curCamera;


struct GLMeshBuffers
{

public:
	GLuint VAO, VBO, EBO, TransformBO;
	vector<Vertex> v;
	vector<GLuint> t;
	GLboolean SupportTransform;
	GLuint ObjectCount;
	Mesh* mesh;
	GLboolean IsBufferDirty;
	GLfloat tSize;
	GLfloat vSize;
	vector<glm::mat4>* BufferStart;
	const std::size_t  sizeofvec4 = sizeof(glm::vec4);
	const std::size_t  sizeofmat4 = sizeof(glm::mat4);
	void Init(Mesh* m, vector<glm::mat4>* transforms)
	{
		//IsBufferDirty = true;
		BufferStart = transforms;
		mesh = m;
		v.insert(v.end(), m->vertices.begin(), m->vertices.end());
		t.insert(t.end(), m->triangles.begin(), m->triangles.end());
		tSize = m->triangles.size();
		vSize=v.size();


		InitBuffer();
	}

	//void Addvt(Mesh* m)
	//{
	//	ObjectCount++;
	//	if (std::find(Meshes.begin(), Meshes.end(), m) == Meshes.end()) {
	//		//IsBufferDirty = true;
	//		v.insert(v.end(), m->vertices.begin(), m->vertices.end());
	//		t.insert(t.end(), m->triangles.begin(), m->triangles.end());
	//		//toffsets.push_back(m->triangles.size());
	//		Meshes.push_back(m);
	//	}
	//	//std::cout << "OC:" << ObjectCount << "VC:" << v.size() << "TC:" << t.size() << "|";
	//	//m->cleartemp();
	//	//m->vertices.insert(m->vertices.end(), m->vertices.begin(), m->vertices.end());
	//	//m->triangles.insert(m->triangles.end(), m->triangles.begin(), m->triangles.end());
	//}
	void AddInstances(vector<glm::mat4>& transforms)
	{
		ObjectCount += transforms.size();
		IsBufferDirty = true;
		glNamedBufferData(TransformBO, ObjectCount * sizeofmat4, &BufferStart->at(0)[0], GL_STATIC_DRAW);
#ifdef Profiler
		VerCount += vSize * transforms.size();
		TrisCount += tSize * transforms.size();
#endif

	}
	void AddInstance(Transform* t)
	{

		ObjectCount++;
		IsBufferDirty = true;
		glNamedBufferData(TransformBO, ObjectCount * sizeofmat4, &BufferStart->at(0)[0], GL_STATIC_DRAW);
#ifdef Profiler
		VerCount += vSize;
		TrisCount += tSize;
#endif
	}
	void UpdatePosBuffer()
	{
		//InitBuffer(TransformArrayStart);
		IsBufferDirty = false;
	
		if (SupportTransform)
		{
			//glBindVertexArray(VAO);
			// получаем указатель
			//std::cout << mesh->name << ": " << BufferStart << " == " << &TransformArray->at(0)[0] << ": " << (BufferStart == &TransformArray->at(0)[0]) << "\n";
			glNamedBufferData(TransformBO, ObjectCount * sizeofmat4, &BufferStart->at(0)[0], GL_STATIC_DRAW);
			//void* ptr = glMapNamedBuffer(TransformBO, GL_WRITE_ONLY);
			//// копируем данные в память
			////float size = sizeof(TransformArray);
			//memcpy(ptr, &BufferStart->at(0)[0],sizeofmat4);
			//// говорим OpenGL, что мы закончили работу с указателем
			//glUnmapNamedBuffer(TransformBO);
	//		glBindVertexArray(VAO);
	///*		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
			
	//
	//		glEnableVertexAttribArray(3);
	//		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	//		glEnableVertexAttribArray(4);
	//		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size));
	//		glEnableVertexAttribArray(5);
	//		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * vec4Size));
	//		glEnableVertexAttribArray(6);
	//		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * vec4Size));

	//		glVertexAttribDivisor(3, 1);
	//		glVertexAttribDivisor(4, 1);
	//		glVertexAttribDivisor(5, 1);
	//		glVertexAttribDivisor(6, 1);


	//		glBindVertexArray(0);
		}
	}
	void UpdatePosBuffer(vector<glm::mat4>* transforms)
	{		//InitBuffer(TransformArrayStart);
		IsBufferDirty = false;
		BufferStart = transforms;
		ObjectCount = transforms->size();
		if (SupportTransform)
		{
			glNamedBufferData(TransformBO, ObjectCount * sizeofmat4, &BufferStart->at(0)[0], GL_STATIC_DRAW);
		}
	}
	void UpdateTransform(Transform* t)
	{
		glNamedBufferSubData(TransformBO, t->id * sizeofmat4, t->id * sizeofmat4 + sizeofmat4, &BufferStart->at(t->id));
	}
//private:
	void InitBuffer()
	{
		IsBufferDirty = false;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Vertex), &v[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, t.size() * sizeof(GLuint),
			&t[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		if (SupportTransform)
		{

			glGenBuffers(1, &TransformBO);
			glBindBuffer(GL_ARRAY_BUFFER, TransformBO);
			glBufferData(GL_ARRAY_BUFFER, ObjectCount * sizeof(glm::mat4), BufferStart, GL_STATIC_DRAW);



			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeofvec4));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeofvec4));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeofvec4));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
		}
		glBindVertexArray(0);

	}
};
interface GlFrameBuffer
{
public: virtual void Use() = 0;
};
struct DepthBuffer : public GlFrameBuffer
{
	GLuint FBO;
	GLuint FBOTex;
	virtual void Resize(int W, int H)
	{
		glBindTexture(GL_TEXTURE_2D, FBOTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, W, H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	//  GlFrameBuffer
	virtual void init(int W, int H)
	{
		//glCreateFramebuffers(1, &FBO);

		//glCreateTextures(GL_TEXTURE_2D, 1, &FBOTex);
		//glTextureParameteri(FBOTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTextureParameteri(FBOTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//glTextureParameteri(FBOTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTextureParameteri(FBOTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTextureStorage2D(FBOTex, 1, GL_DEPTH_COMPONENT, W, H);
		////glTextureSubImage2D(FBOTex, 0, 0, 0, W, H, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
		//glNamedFramebufferTexture(FBO,GL_DEPTH_ATTACHMENT, FBOTex,0);
		//glNamedFramebufferDrawBuffer(FBO, GL_NONE);
		//glNamedFramebufferReadBuffer(FBO, GL_NONE);
		glGenFramebuffers(1, &FBO);
		// create depth texture
		glGenTextures(1, &FBOTex);
		glBindTexture(GL_TEXTURE_2D, FBOTex);
		glObjectLabel(GL_TEXTURE, FBOTex, -1, std::format("DepthBuffer Id: {}", FBOTex).c_str());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, W, H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBOTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//  GlFrameBuffer
	virtual void Use() override
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
//struct OcclusionBuffer : public GlFrameBuffer
//{
//	GLuint FBO;
//	GLuint FBOTex;
//	unsigned int DownScale = 2;
//	virtual void init(int downScale) 
//	{
//		DownScale = downScale;
//		glGenFramebuffers(1, &FBO);
//		// create depth texture
//		glGenTextures(1, &FBOTex);
//		glBindTexture(GL_TEXTURE_2D, FBOTex);
//		glObjectLabel(GL_TEXTURE, FBOTex, -1, std::format("Occlusion Id: {}", FBOTex).c_str());
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, SCR_WIDTH* downScale, SCR_HEIGHT* downScale, 0, GL_RG_INTEGER, GL_UNSIGNED_INT, NULL);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		// attach depth texture as FBO's depth buffer
//		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_UNSIGNED_INT, GL_TEXTURE_2D, FBOTex, 0);
//		//glDrawBuffer(GL_NONE);
//		//glReadBuffer(GL_NONE);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	}
//	//  GlFrameBuffer
//	virtual void Use() override
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//};
DepthBuffer CameraDepth = DepthBuffer();
class LightSource :public GameObject
{
public:
	float Power;
	glm::vec3 Color;
public:
	virtual void Render() = 0;
};
#pragma endregion



#pragma region Managers
class InputManager
{
	GLFWwindow* window;
	Dictionary<GLuint, bool> KeyPreviousState;
public:
	inline GLboolean IsKeyHold(GLuint key)
	{

		return KeyPreviousState[key] = glfwGetKey(window, key);
		//== GLFW_RELEASE   GLFW_PRESS
	}
	inline  bool IsKeyDown(GLuint key)
	{
		bool t = KeyPreviousState[key];
		//std::cout << (t != (KeyPreviousState[key] = glfwGetKey(window, key)) ) << "|";

		if (t
			!=
			(KeyPreviousState[key] = glfwGetKey(window, key))
			&&
			KeyPreviousState[key] == GLFW_PRESS)
			return GLFW_PRESS;
		return GLFW_RELEASE;
	}
	inline bool IsKeyUp(GLuint key)
	{
		bool t = KeyPreviousState[key];
		if (t
			!=
			(KeyPreviousState[key] = glfwGetKey(window, key))
			&&
			KeyPreviousState[key] == GLFW_RELEASE)
			return GLFW_PRESS;
		return GLFW_RELEASE;
	}
public:
	InputManager(GLFWwindow* w)
	{
		window = w;
	}
};

static class rm
{
public:

	static void init()
	{
		DeffaultTexture::init();
		DeffaultShader::init();
	}
};
#pragma endregion


#pragma region Main.h
void processInput(GLFWwindow* window);
void DrawGUI(GLFWwindow* window);
void ReleaseAll(GLFWwindow* window);
#pragma region CallBacks
void OnWindowResize(GLFWwindow* window, int width, int height);
void MouseScrollCallback(GLFWwindow* window, GLdouble x, GLdouble y);
void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
#pragma endregion
void renderQuad();
void renderCube();
#pragma endregion
#pragma region Main.cpp



#pragma region Variables 
// settings
GLuint SCR_WIDTH = 1600;
GLuint SCR_HEIGHT = 900;


GLfloat totalTime = 0;
GLfloat deltaTime = 0;
GLuint frames = 0;
GLfloat frameRate = 30;
GLfloat Rotation = 20;
GLfloat dt = 0;
GLfloat  averageFrameTimeMilliseconds = 33.333;

#ifdef Profiler
float CPUTime;
float GPUTime;

#endif
const float ns2ms = 1.f / 1000000.f;
const float sec2ms = 1.f / 1000.f;
const float bit2mb = 1.19e-7;

glm::mat4 projection = glm::perspective(glm::radians(65.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 2100.0f);

GLboolean IsProjectionDirty = 1;
GLboolean IsviewDirty = 1;

GLuint gBuffer;
GLuint rboDepth;
GLuint gPosition, gNormal, gAlbedoSpec;

GLfloat Quadratic = 0.1f;
GLfloat Linear = 0.1f;
GLfloat precision = 0.2;

float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool DebugShaders = true;

glm::vec2 shadowRes = glm::vec2(SCR_WIDTH, SCR_HEIGHT);

bool backCull = false;

GLboolean VSync = 1;
GLboolean XRay = 0;
bool Move = 1;
bool DSided = 1;
bool DrawOcclusionBoxes= 0;

Shader_CN st;
Camera* camera;/*=new*/
InputManager* Input;

//...
Material* MatInUse;
vector<Mesh*>                    Meshes;
Dictionary <Mesh*, vector<Material*>* > InstancedMeshes;
Dictionary<Mesh*, GLMeshBuffers> InstancedMeshsBuffers;
Dictionary<Transform*, GLMeshBuffers*> TransformBuffers;
Dictionary<Mesh*, vector<glm::mat4>> InstancedMaterialsTransforms;
//Dictionary < Material* ,vector <GLMeshBuffers>>
vector<RenderedObject*> AllSpawnedObjects;
vector<Shader*> UsedShaders;
std::unordered_set <GLMeshBuffers*> DirtyBuffers;
std::unordered_set <Transform*> DirtyTransforms;
//vector<Transfo>




#pragma endregion
void AddToRender(RenderedObject* r)
{
	vector<glm::mat4>* mat = &InstancedMaterialsTransforms[r->mesh];
	GLMeshBuffers* gb = &InstancedMeshsBuffers[r->mesh];
	glm::mat4 model = glm::mat4(1.0f);
	float angle = 0.0f;
	mat->push_back(model);
	r->transform.modelMatrixPtr = mat;
	r->transform.RecalcMatrix();
	r->transform.id = mat->size()-1;
	if (!InstancedMeshes.contains(r->mesh))
	{
		Meshes.push_back(r->mesh);
		InstancedMeshes[r->mesh] = &r->material;
		gb->SupportTransform = true;
		gb->Init(r->mesh, &InstancedMaterialsTransforms[r->mesh]);
	}
	gb->AddInstance(&r->transform);
	AllSpawnedObjects.push_back(r);
	TransformBuffers[&r->transform] = gb;



	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));


	//mat->push_back(glm::mat4(1.0f));


	//r->RecalculateTransform();


	//gb->UpdateBuffers(InstancedMaterialsTransforms[r->material]);
}
inline void RenderInstanceMesh(Mesh* m, glm::mat4 ProjViewMatrix)
{
	vector<Material*>* mats = InstancedMeshes[m];
	GLMeshBuffers* b = &InstancedMeshsBuffers[m];
	//InstancedMaterialsTransforms[m]
	if (b->IsBufferDirty)b->UpdatePosBuffer();

	//std::cout << "Shader id:" << m->shader->MainId;
	//m->shader->use();
	Material* mat = mats->at(0);
	int i = 0;
	glBindVertexArray(b->VAO);
	int count = b->mesh->subMeshes.size();
	for (auto sm : b->mesh->subMeshes)
	{
		if (mats->size() >i)
		mat = mats->at(i);
		if (mat != MatInUse)
		{
			mat->UseMaterial();
			MatInUse = mat;
			if (b->SupportTransform)
			{
				glUniformMatrix4fv(glGetUniformLocation(mat->shader->MainId, "projViewMatrix"), 1, GL_FALSE, &ProjViewMatrix[0][0]);
			}
		}
		//glDrawArraysInstanced(GL_TRIANGLES, sm, (((i + 1) >= count) ? b->tSize : b->mesh->subMeshes[i+1]), b->ObjectCount);
		glDrawElementsInstanced(
			GL_TRIANGLES, (((i + 1) >= count) ? b->tSize : b->mesh->subMeshes[i + 1]), GL_UNSIGNED_INT,(void*)sm, b->ObjectCount
		);
		i++;


		//std::cout << p[0][0] << p[0][1] <<  p[0][2] << p[0][3] <<"\n" << p[1][0] << p[1][1] << p[1][2] << p[1][3] << "\n" << p[2][0] << p[2][1] << p[2][2] << p[2][3] << "\n" << p[3][0] << p[3][1] << p[3][2] << p[3][3] << "\n";
		//std::cout << "=======" << "\n";

	
		//GLuint offset = 0;
		//glDrawArrays(GL_TRIANGLES, offset, offset += b->toffsets[0]);

			//glDrawArrays(GL_TRIANGLES, offset, offset += b->toffsets[0]);
		//for ()

	}

	//glDrawArrays(GL_TRIANGLES, offset, offset+=b->toffsets[i]);

}
inline void RenderScene(RenderShader shader, Camera* cam)
{
	CurrentShaderType = shader;
	GLuint count = Meshes.size();
	for (auto t : DirtyBuffers)
	{
		t->UpdatePosBuffer();
	}
	for (auto t : DirtyTransforms)
	{
		t->RecalcMatrix();
	}
	
	DirtyBuffers.clear();
	DirtyTransforms.clear();
	//RenderScene(shader, cam->projection * cam->viewMatrix);
	for (GLuint i = 0; i < count; i++)
	{
		RenderInstanceMesh(Meshes[i], cam->projection * cam->viewMatrix);
	}
}
inline void RenderScene(RenderShader shader, glm::mat4& ViewMatrix, glm::mat4& projectionMatrix)
{
	CurrentShaderType = shader;
	GLuint count = Meshes.size();

	for (GLuint i = 0; i < count; i++)
	{
		RenderInstanceMesh(Meshes[i], projectionMatrix * ViewMatrix);
	}
}
inline void TransformChanged(Transform* t) { DirtyBuffers.insert(TransformBuffers[t]); DirtyTransforms.insert(t); }

inline void ForceRender(GLMeshBuffers* glmb, Material* mat, glm::mat4 ProjViewMatrix)
{
	glBindVertexArray(glmb->VAO);
	int i = 0;
	int count = glmb->mesh->subMeshes.size();
	for (auto sm : glmb->mesh->subMeshes)
	{
		if (mat != MatInUse)
		{
			mat->UseMaterial();
			MatInUse = mat;
			if (glmb->SupportTransform)
			{
				glUniformMatrix4fv(glGetUniformLocation(mat->shader->MainId, "projViewMatrix"), 1, GL_FALSE, &ProjViewMatrix[0][0]);
			}
		}
		//glDrawArraysInstanced(GL_TRIANGLES, sm, (((i + 1) >= count) ? b->tSize : b->mesh->subMeshes[i+1]), b->ObjectCount);
		glDrawElementsInstanced(
			GL_TRIANGLES, (((i + 1) >= count) ? glmb->tSize : glmb->mesh->subMeshes[i + 1]), GL_UNSIGNED_INT, (void*)sm, glmb->ObjectCount
		);
		i++;

	}
}
inline void ForceRender(GLMeshBuffers* glmb, Material* mat, glm::mat4& ViewMatrix, glm::mat4& projectionMatrix)
{
	ForceRender(glmb, mat, projectionMatrix* ViewMatrix);
}
inline void ForceRender(GLMeshBuffers* glmb, Material* mat, Camera* cam)
{
	ForceRender(glmb,mat,cam->projection * cam->viewMatrix);
}

inline void InitShader(Shader& s)
{
	s.init(); s.Compile(DebugShaders);
	UsedShaders.push_back(&s);
}
inline GLMeshBuffers CreateMeshBuffer(Mesh* m,vector<glm::mat4>*  transforms)
{
	GLMeshBuffers gb = GLMeshBuffers();
	gb.SupportTransform = true;
	gb.Init(m, transforms);
	return gb;
}

inline void TransfomMove(Transform* t, glm::vec3& delta) { t->AddPosition(delta); TransformChanged(t); }
inline void TransfomMove(Transform* t, glm::vec3 delta) { t->AddPosition(delta); TransformChanged(t); }

#pragma region SSPostProc

float DRLOffest = 0.1;
float DRLOffest2 = 100;

class DirectionalLight :public LightSource
{
public:
	glm::mat4 LightProj;
	//glm::mat4 LightView;
	glm::mat4 LightMatrix;
	Camera* cam;
	GLfloat H;
	GLfloat W;
	DepthBuffer db;
	DirectionalLight(glm::vec2 Resolution)
	{
		H = Resolution.y;
		W = Resolution.x;
		name = "Directional Light";
		transform.position = glm::vec3(-2.f, 100.f, -1.f);
		transform.scale = glm::vec3(1.f);
		//camera->Position = position;
		/*LightProj = glm::ortho<float>(-110.f, 110.f, -110.f, 110.f, 0.1f, 115.f);*/
		LightProj = glm::ortho<float>(-128.f, 128.f, -128.f, 128.f, 0.1f, 256.f);

		allCameras.Add(Camera(LightProj,"DirectionalLight", transform.position));
		cam = &allCameras.data[allCameras.count - 1];
		cam->LookAt(camera->Position);
		//LightView =camera->GetViewMatrix() /*glm::lookAt(position, position + glm::normalize(glm::vec3(0) - position), vec3Up)*/;
		//camera->Position = glm::vec3(0);
		//LightView = glm::mat4(1.0f);
		//LightView = glm::translate(LightView, position);
		//LightView = glm::rotate(LightView, glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
		//LightView = glm::rotate(LightView, glm::radians(-30.f), glm::vec3(0.f, 1.f, 0.f));
		//LightView = glm::scale(LightView, glm::vec3(1.f));
		//LightProj = glm::perspective(glm::radians(70.0f), (GLfloat)W / (GLfloat)H, 0.1f, 100.0f);/*glm::ortho<float>(-10.f, 10.f, -10.f, 10.f, 0.1f, 1000.f)*/;
		//LightView = glm::lookAt(position, glm::vec3(0.1f,0.1f,0.1f), glm::vec3(0.f, 1.f, 0.f));
		//LogMatrix(LightView,"LightView");
		//LogMatrix(LightProj, "LightProj");
		//LightMatrix = LightView* LightProj ;
		//LogMatrix(LightMatrix, "LightMatrix");
		db.init(W, H);
	}
	void Resize(glm::vec2 Resolution)
	{
		H = Resolution.y;
		W = Resolution.x;
		db.Resize(W, H);
	}

	// LightSource
	virtual void Render() override
	{
		//cam->Position = camera->Position + transform.position;
		cam->updateCameraVectors();
		glViewport(0, 0, W, H);
		db.Use();



		if (backCull) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(DRLOffest, DRLOffest2);
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_ALWAYS);
		RenderScene(DEPTH, cam);
		glDisable(GL_POLYGON_OFFSET_FILL);
		if (backCull) {
			glDisable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
	}

};

class ScreenSpacePostProcessing
{
public:
	bool enabled;
	virtual void init() = 0;
	virtual void Prepare() = 0;
	virtual void Render() = 0;
};
class DefferedLight :public ScreenSpacePostProcessing
{
public:
	GLuint DepthBuffer;
	GLuint GBuffer;

	GLuint gPosition, gNormal, gAlbedoSpec;
	const unsigned int NR_LIGHTS = 4;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	LightShader ls;

	DefferedLight(GLuint depthBuffer, GLuint gBuffer, GLuint gPos, GLuint gNorm, GLuint gAlbSpec)
	{
		DepthBuffer = depthBuffer;
		GBuffer = gBuffer;
		gPosition = gPos;
		gNormal = gNorm;
		gAlbedoSpec = gAlbSpec;
		ls.init();
		ls.Compile(true);
	}
	void init() override
	{
		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{

			lightPositions.push_back(Randomv3(-5.12f - i / 1.f, 5.12f + i / 1.f));
			// also calculate random color
			float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
			float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
			float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
			lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}

		ls.use();
		ls.dynamicSetInt("gPosition", 0);
		ls.dynamicSetInt("gNormal", 1);
		ls.dynamicSetInt("gAlbedoSpec", 2);
	}
	void Prepare() override
	{

	}
	void Render() override
	{

		ls.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		stringstream ss;
		// send light relevant uniforms
		const float constant = 1.0f;
		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{			// update attenuation parameters and calculate radius
			// // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			//const float linear = 0.1f;
			//const float quadratic = 0.1f;
			ss.str("");

			string base = "lights[" + std::to_string(i) + "].";

			ls.setVec3((base + "Position").c_str(), lightPositions[i]);
			ls.setVec3((base + "Color").c_str(), lightColors[i]);

			ls.dynamicSetFloat((base + "Linear").c_str(), Linear);
			ls.dynamicSetFloat((base + "Quadratic").c_str(), Quadratic);

			// then calculate radius of light volume/sphere
			const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
			float radius = (-Linear + std::sqrt(Linear * Linear - 4 * Quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * Quadratic);

			ls.dynamicSetFloat((base + "Radius").c_str(), radius);
		}
		ls.setVec3("viewPos", glm::vec3(0.0f, 0.0f, -4.0f));
		// finally render quad
		renderQuad();


	}
};
class ScreenSpaceShadows : public ScreenSpacePostProcessing
{
	ScreenSpaceShadowShader ssss;
public:
	/*DirectionalLight dl = DirectionalLight(glm::vec2(2048.f, 2048.f));*/
	DirectionalLight dl = DirectionalLight(shadowRes);

public:
	virtual void init() override
	{
		InitShader(ssss);
		//dl = DirectionalLight(glm::vec2(2048, 2048));
	}
	virtual void Prepare() override
	{
		if (dl.W != shadowRes.x || dl.H != shadowRes.y) dl.Resize(shadowRes);
		dl.Render();
	}
	virtual void Render() override
	{
		//uniform sampler2D shadowMap;
		//uniform sampler2D cameraDepth;
		//uniform mat4 lightProjMatrix;
		//uniform mat4 lightViewMatrix;
		ssss.use();
		ssss.setMat4("inv_view_proj_matrix", glm::inverse(camera->viewMatrix * projection));
		ssss.setMat4("inv_lightSpaceMatrix", glm::inverse(dl.cam->viewMatrix * dl.cam->projection));
		ssss.setMat4("view_matrix", camera->viewMatrix);
		ssss.setMat4("proj_matrix", projection);
		//LogMatrix(dl.LightProj);
		//Sleep(2250);
		//ssss.setMat4("lightProjMatrix", projection);
		//camera->Position += vec3Up	;
		//camera->Yaw += 12;
		//ssss.setMat4("lightViewMatrix", camera->GetViewMatrix());		
		//camera->Yaw -= 12;
		//camera->Position -= vec3Up;
		ssss.setMat4("lightProjMatrix", dl.cam->projection);
		ssss.setMat4("lightViewMatrix", dl.cam->viewMatrix);
		// 
		//ssss.setInt("cameraDepth", CameraDepth.FBOTex);
		glActiveTexture(GL_TEXTURE0 + 0);
		glUniform1i(glGetUniformLocation(ssss.MainId, "cameraDepth"), 0);
		glBindTexture(GL_TEXTURE_2D, CameraDepth.FBOTex);
		//ssss.setInt("shadowMap", dl.db.FBOTex);
		glActiveTexture(GL_TEXTURE0 + 1);
		glUniform1i(glGetUniformLocation(ssss.MainId, "shadowMap"), 1);
		//glBindTexture(GL_TEXTURE_2D, dl.db.FBOTex);
		glBindTexture(GL_TEXTURE_2D, dl.db.FBOTex);

		glUniform1f(glGetUniformLocation(ssss.MainId, "shadowPrecision"), precision);

		// set light uniforms
		ssss.setVec3("viewPos", camera->Position);
		ssss.setVec3("lightPos", dl.transform.position);
		//glViewport(0, 0, 2048, 2048);
		renderQuad();
		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	}
};
vector<ScreenSpacePostProcessing*> sspp;
#pragma endregion









int main()
{
	auto full = std::chrono::high_resolution_clock::now();

#pragma region Window
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGlVersionMa);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGlVersionMi);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
#ifdef Debug 
		std::cout << "Failed to create GLFW window" << std::endl;
#endif
		glfwTerminate();
		return -1;
	}



	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnWindowResize);
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
#ifdef Debug 
		std::cout << "Failed to initialize GLAD" << std::endl;
#endif
		return -1;
	}
#pragma endregion


#pragma region GUI
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(OpenGlShaderVersion.c_str());
#pragma endregion

#pragma region  Audio
	//FMOD::System::loadPlugin();

	//IPLContextSettings contextSettings{};
	//contextSettings.version = STEAMAUDIO_VERSION;

	//IPLContext context = nullptr;
	//iplContextCreate(&contextSettings, &context);

	//IPLHRTFSettings hrtfSettings{};
	//hrtfSettings.type = IPL_HRTFTYPE_DEFAULT;

	//IPLAudioSettings audioSettings{};
	//audioSettings.samplingRate = 44100;
	//audioSettings.frameSize = 1024;

	//IPLHRTF hrtf = nullptr;
	//iplHRTFCreate(context, &audioSettings, &hrtfSettings, &hrtf);

	//IPLBinauralEffectSettings effectSettings{};
	//effectSettings.hrtf = hrtf;

	//IPLBinauralEffect effect = nullptr;
	//iplBinauralEffectCreate(context, &audioSettings, &effectSettings, &effect);

	//std::vector<float> inputaudio =  { 0.1f, 0.2f, 0.3f, 0.1f, 0.2f };
	//std::vector<float> outputaudio;

	//float* inData[] = { inputaudio.data() };

	//IPLAudioBuffer inBuffer{};
	//inBuffer.numChannels = 1;
	//inBuffer.numSamples = audioSettings.frameSize;
	//inBuffer.data = inData;
	//IPLAudioBuffer outBuffer{};
	//iplAudioBufferAllocate(context, 2, audioSettings.frameSize, &outBuffer);

	//std::vector<float> outputaudioframe(2 * audioSettings.frameSize);
#pragma endregion




	auto start = std::chrono::high_resolution_clock::now();
#ifdef MultuThreadTexLoad
	hglrc = wglGetCurrentContext();
	hdc = wglGetCurrentDC();
	Shglrc = wglCreateContext(hdc);
	wglShareLists(Shglrc, hglrc);
#endif
	Parallel.init();
#ifdef Debug
	LogTotalTime(start);
	std::cout << "^=================  Parallel   =================^" << "\n";
 #endif
	Camera cam = Camera(projection,"Main", glm::vec3(0.0f, 0.0f, 5.0f));
	allCameras.Add(cam);
	camera = &allCameras.data[0];
	rm::init();
	Input = new InputManager(window);
#pragma region Resources
#ifdef Debug 
	start = std::chrono::high_resolution_clock::now();
	std::cout << "v=================   Shader Log   =================v" << "\n\n";
#endif
	Shader s = Shader(); InitShader(s);
	Shader3d s3d = class Shader3d(); InitShader(s3d);
	st = Shader_CN(); InitShader(st);
	LightShader ls = LightShader(); InitShader(ls);
#ifdef Debug 
	LogTotalTime(start);
	std::cout << "^=================   Shader Log   =================^" << "\n";
	//st.use();
	std::cout << "v=================  Texture Log   =================v" << "\n\n"; start = std::chrono::high_resolution_clock::now();
#endif
	CompressionMethod cm = None;
#ifdef MultuThreadTexLoad

	Texture2d t0 = Texture2d("\\Resources\\Images\\checker-map_tho.png", albedo, cm); Parallel.LoadTexture(&t0);
	Texture2d t1 = Texture2d("\\Resources\\Images\\texture1.jpg", albedo, cm); Parallel.LoadTexture(&t1);
	Texture2d t2 = Texture2d("\\Resources\\Images\\texture2.jpg", albedo, cm); Parallel.LoadTexture(&t2);
	Texture2d t3 = Texture2d("\\Resources\\Images\\texture3.jpg", albedo, cm); Parallel.LoadTexture(&t3);
	//Texture2d t4 = Texture2d("\\Resources\\Images\\Brick.jpg", albedo, Long);
	Texture2d t4 = Texture2d("\\Resources\\Images\\Brick2.png", albedo, cm); Parallel.LoadTexture(&t4);
	Texture2d t5 = Texture2d("\\Resources\\Images\\Grassv2.jpg", albedo, cm); Parallel.LoadTexture(&t5);
	Texture2d t6 = Texture2d("\\Resources\\Images\\hoodwink_body_color_psd_67916807_RGB.png", albedo, cm); Parallel.LoadTexture(&t6);
	Texture2d t7 = Texture2d("\\Resources\\Images\\hoodwink_head_color_psd_4e84f665_RGB.tga", albedo, cm); Parallel.LoadTexture(&t7);
	
	//
#else
	Texture2d t0 = Texture2d("\\Resources\\Images\\checker-map_tho.png", albedo, cm);
	Texture2d t1 = Texture2d("\\Resources\\Images\\texture1.jpg", albedo, cm);
	Texture2d t2 = Texture2d("\\Resources\\Images\\texture2.jpg", albedo, cm);
	Texture2d t3 = Texture2d("\\Resources\\Images\\texture3.jpg", albedo, cm);
	//Texture2d t4 = Texture2d("\\Resources\\Images\\Brick.jpg", albedo, Long);
	Texture2d t4 = Texture2d("\\Resources\\Images\\Brick2.png", albedo, cm);
	Texture2d t5 = Texture2d("\\Resources\\Images\\Grassv2.jpg", albedo, cm);
	Texture2d t6 = Texture2d("\\Resources\\Images\\hoodwink_body_color_psd_67916807_RGB.tga", albedo, cm); 
	Texture2d t7 = Texture2d("\\Resources\\Images\\hoodwink_head_color_psd_4e84f665_RGB.tga", albedo, cm); 
#endif
#ifdef Debug 
	LogTotalTime(start);
	std::cout << "^=================  Texture Log   =================^" << "\n";

	std::cout << "v=================   Materials Log   =================v" << "\n\n";   start = std::chrono::high_resolution_clock::now();
#endif
	
	Material_CNShader mat1 = Material_CNShader(&st, &t2, &t3, glm::vec2(1, 1));
	Material_CNShader mat3 = Material_CNShader(&st, &t4, &t1, glm::vec2(1, 1));
	Material_CNShader mat6 = Material_CNShader(&st, &t6, &t1, glm::vec2(1, 1));
	Material_CNShader mat7 = Material_CNShader(&st, &t7, &t1, glm::vec2(1, 1));
	
	float PlaneSize = 100.f;
	Material_CNShader mat2 = Material_CNShader(&st, &t5, &t5, glm::vec2(PlaneSize, PlaneSize));
	Material mat0 = Material(&s);
#ifdef Debug 
	LogTotalTime(start);
	std::cout << "^=================   Materials Log   =================^" << "\n";

	std::cout << "v=================   Meshes Log   =================v" << "\n\n";      start = std::chrono::high_resolution_clock::now();
#endif
	Mesh m3 = Mesh();
	//PRC_Cube().LoadMesh(&m3.vertices, &m3.triangles);
	m3.LoadFromFile("Resources\\Models\\untitled.obj");
	Mesh hoodwinkMesh = Mesh(); hoodwinkMesh.LoadFromFile(
"Resources\\Models\\Hoodwink.obj"
	);
	Mesh m2 = M_Sphere(0.2f, 24, 12);
	Mesh m1 = M_Cube(0.6f);
	Mesh m = M_NoisePlane(PlaneSize, PlaneSize, 25.5f);
#ifdef Debug 
	LogTotalTime(start);
	std::cout << "^=================   Meshes Log   =================^" << "\n";

#pragma endregion

	std::cout << "v=================   Creating RendObj   =================v" << "\n\n";
	
	start = std::chrono::high_resolution_clock::now();
#endif
	AddToRender(new RenderedObject(m, mat2, glm::vec3(-PlaneSize/2, -4.f, -PlaneSize/2)));
	
	GLuint count = 1234;
	for (int i = 0; i < count; i++)
	{
		AddToRender(new RenderedObject(m2, mat3, Randomv3(-13.12f - i / 210.f, 13.12f + i / 210.f)));
	}
	for (int i = 0; i < count; i++)
	{
		AddToRender(new RenderedObject(m3, mat1, Randomv3(-5.12f - i / 270.f, 5.12f + i / 270.f)));
	}
	//for (int i = 0; i < count; i++)
	//{
	//	//AddToRender(new RenderedObject(m1, mat2, Randomv3(-5.12f - i / 125.f, 5.12f + i / 125.f)));
	//	AddToRender(new RenderedObject(hoodwinkMesh, vector<Material*> { &mat7, & mat6 }, Randomv3(-5.12f - i / 125.f, 5.12f + i / 125.f)));
	//}
	//AddToRender(new RenderedObject(hoodwinkMesh, vector<Material*> { &mat7, & mat6 }, glm::vec3(34, 10, 34)));

	AddToRender(new RenderedObject(hoodwinkMesh, vector<Material*> { &mat7, & mat6 }, glm::vec3(10, 5, 10)));
	AddToRender(new RenderedObject(m2, mat3, glm::vec3(6, 5, 6)));
	AddToRender(new RenderedObject(m3, mat1, glm::vec3(2, 5, 2)));
	//AddToRender(new RenderedObject(m1, mat2, glm::vec3(-3, 5, -3)));
	
#pragma region Buffers
	CameraDepth.init(SCR_WIDTH, SCR_HEIGHT);

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);


	//position color buffer
	//glGenTextures(1, &gPosition);
	//glBindTexture(GL_TEXTURE_2D, gPosition);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	////normal color buffer
	//glGenTextures(1, &gNormal);
	//glBindTexture(GL_TEXTURE_2D, gNormal);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	////color + specular color buffer
	//glGenTextures(1, &gAlbedoSpec);
	//glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	////tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	//unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	//glDrawBuffers(3, attachments);


	//===============================================================================================


	 //create and attach depth buffer (renderbuffer) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! uncomment next 4 lines if smth broken

	//glGenRenderbuffers(1, &rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//finally check if framebuffer is complete
#ifdef Debug
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;  
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


#pragma endregion
#ifdef Debug 
	LogTotalTime(start);
	std::cout << "v==============   Post Processing Log   ==============v" << "\n\n";      start = std::chrono::high_resolution_clock::now();
#endif
	//DefferedLight dl = DefferedLight(gBuffer, rboDepth, gPosition, gNormal, gAlbedoSpec);
	//dl.init();
	//sspp.push_back(&dl);
	ScreenSpaceShadows sss = ScreenSpaceShadows();	sss.init();	sspp.push_back(&sss);

#ifdef Debug 
	std::cout << "^==============   Post Processing Log   ==============^" << "\n";
	std::cout << "v  Full startup time  v" << "\n";
	LogTotalTime(full);
#endif

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSwapInterval(1);

	glm::mat4 cp;
	GLuint matCount = 0;
	camera = &allCameras.data[0];
#ifdef Profiler
	auto ProfilerTimer = std::chrono::high_resolution_clock::now();
	auto ProfilerFinish = std::chrono::high_resolution_clock::now();
#endif

	vector <glm::mat4> Bbtransf = vector<glm::mat4>();
	Mesh Bb = M_Cube(1.f);
	GLMeshBuffers BbB = CreateMeshBuffer(&Bb, &Bbtransf);
	Material_CNShader BbM = Material_CNShader(&st, &t1, &t1, glm::vec2(1, 1));
	glm::mat4 tmpMat;
	//while (TexturesToLoad > 0);
	while (!glfwWindowShouldClose(window))
	{
		auto start = std::chrono::high_resolution_clock::now();

		processInput(window);

#pragma region Update
#ifdef Profiler
		ProfilerTimer = std::chrono::high_resolution_clock::now();
#endif

		//glBindBuffer(GL_ARRAY_BUFFER, pos_Buf);
		//data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//data[7] = 0.4f;
		//glUnmapBuffer(GL_ARRAY_BUFFER);
		if (DrawOcclusionBoxes)
		{
			Bbtransf.clear();
			for (auto v : AllSpawnedObjects)
			{
				
				/*tmpMat = glm::mat4(1.0f);*/
				Bbtransf.push_back(
					glm::translate(glm::mat4(1.0f),  v->transform.position + v->mesh->pivot) *
					/*glm::toMat4(v->transform.qRot) * */
					glm::scale(glm::mat4(1.0f), v->mesh->bouds * v->transform.scale));
					
			}
		}


		if(Move)
		{
			GLuint roc = AllSpawnedObjects.size();
			for (GLuint i = 1; i < roc; i++)
			{
				//glm::vec3 start = glm::vec3(sin(totalTime * glm::sqrt(i) / 2), 0,cos(totalTime *glm::sqrt(i) /2));
				////glm::vec3 endstart = glm::vec3(sin(totalTime), cos(totalTime), sin(totalTime * cos(totalTime)));
				//glm::vec3 res =Randomv3(start*0.45f, start*1.55f);
				//glm::vec3 start = glm::vec3(RandomGLf(sin(totalTime * glm::sqrt(i) / 2), 1.25f,1),0, RandomGLf(cos(totalTime * i), 1.25,1));
				//glm::vec3 end = glm::vec3(RandomGLf(cos(totalTime*i), 1.25, 1),0, RandomGLf(sin(totalTime * glm::sqrt(i) / 2), 1.25f, 1));
				//glm::vec3 res = Randomv3(-start, end);
				glm::vec3 start = glm::vec3(0,sin(totalTime + (glm::sqrt(i) / 2.f)),0);
				//glm::vec3 end = glm::vec3(RandomGLf(cos(totalTime * i), 1.25, 1), 0, RandomGLf(sin(totalTime * glm::sqrt(i) / 2), 1.25f, 1));
				//glm::vec3 res = Randomv3(-start, end);

				TransfomMove(&AllSpawnedObjects[i]->transform, start * (dt * 5));
				//AllSpawnedObjects[i]->transform.AddPosition(start * (dt*5));
				//AllSpawnedObjects[i]->transform.RecalcMatrix();
			}
		}
		//LogMatrix(AllSpawnedObjects[6]->transform.GetMatrixPtr(), "Transform Before");

		//LogMatrix(AllSpawnedObjects[6]->transform.GetMatrixPtr(), "Transform After");
		//Sleep(5);
#ifdef Profiler
		ProfilerFinish = std::chrono::high_resolution_clock::now();
		CPUTime = std::chrono::duration_cast<std::chrono::nanoseconds>(ProfilerFinish - ProfilerTimer).count() * ns2ms;
#endif
#pragma endregion

#ifdef Profiler
		ProfilerTimer = std::chrono::high_resolution_clock::now();
#endif

		if (curCamera == 1) {

			glClearColor(0.6f, 0.8f, 0.9f, 0.6f);
		}
		//glClearDepth(0.5f);
		glClearColor(0.6f, 0.8f, 0.9f, 0.6f);
		

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint ppc = sspp.size();
		for (GLuint i = 0; i < ppc; i++)
		{
			sspp.at(i)->Prepare();
		}


		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		cp = camera->GetViewMatrix();
		
		CameraDepth.Use();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderScene(DEPTH, camera);



		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//RenderScene(SCREEN,cp, projection);
		//if (backCull)glCullFace(GL_FRONT);
		RenderScene(SCREEN, camera);
		if (DrawOcclusionBoxes)
		{
			//if (!XRay)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			BbB.UpdatePosBuffer(&Bbtransf);
			ForceRender(&BbB, &BbM, camera);

			//if (!XRay)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//if (backCull)glCullFace(GL_BACK);



		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glDisable(GL_DEPTH_TEST);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GLuint i = 0; i < ppc; i++)
		{
			if (sspp.at(i)->enabled)
				sspp.at(i)->Render();
		}

		DrawGUI(window);

		if (IsProjectionDirty)IsProjectionDirty = false;
		//===========================================











		glfwSwapBuffers(window);







#ifdef Profiler
		ProfilerFinish = std::chrono::high_resolution_clock::now();
		GPUTime = std::chrono::duration_cast<std::chrono::nanoseconds>(ProfilerFinish - ProfilerTimer).count() * ns2ms;
#endif
		//Sleep(500);
		glfwPollEvents();

#pragma region Fps
		auto finish = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / 1000000000.0;
		deltaTime += dt;
		totalTime += dt;
		frames++;
#ifdef Profiler
		if (deltaTime > 0.25) { //every second

			frameRate = (GLfloat)frames * 0.5 * 4 + frameRate * 0.5; //more stable
			averageFrameTimeMilliseconds = 250.0 / frames;
			frames = 0;
			deltaTime -= 0.25;




			/*std::ostringstream ss1;
			ss1 << "FPS:" << frameRate << "avg. Ms:" << averageFrameTimeMilliseconds << "sp: " << precision;

			glfwSetWindowTitle(window, ss1.str().c_str());*/
		}
#endif
#pragma endregion
		//Sleep(1000);

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//count = Meshes.size();

	for (auto s : UsedShaders)
		glDeleteProgram(s->MainId);
	
	ReleaseAll(window);
	glfwTerminate();
	return 0;
}


GLfloat Boost = 1;
inline void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (Input->IsKeyDown(GLFW_KEY_V))
	{
		if (VSync == 0) { VSync = 1; glfwSwapInterval(1); }
		else
			if (VSync == 1) { VSync = 0; glfwSwapInterval(0); }
	}
	if (Input->IsKeyDown(GLFW_KEY_X))
	{
		XRay = !XRay;
		if (!XRay)  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}


	if (Input->IsKeyDown(GLFW_KEY_Q)) { curCamera++; if (curCamera >= allCameras.count)curCamera = 0; camera = &allCameras.data[curCamera]; }
	if (Input->IsKeyDown(GLFW_KEY_B))backCull = !backCull;
	Boost = 1;
	if (Input->IsKeyHold(GLFW_KEY_LEFT_SHIFT))Boost = 3.25f;
	if (Input->IsKeyHold(GLFW_KEY_W))camera->Position += camera->Front * 2.f * dt * Boost;
	if (Input->IsKeyHold(GLFW_KEY_A))camera->Position += camera->Right * -2.f * dt * Boost;
	if (Input->IsKeyHold(GLFW_KEY_S))camera->Position += camera->Front * -2.f * dt * Boost;
	if (Input->IsKeyHold(GLFW_KEY_D))camera->Position += camera->Right * 2.f * dt * Boost;
	if (Input->IsKeyHold(GLFW_KEY_E))Rotation += 125.0 * dt;
	if (Input->IsKeyHold(GLFW_KEY_Q))Rotation -= 125.0 * dt;
	if (Input->IsKeyDown(GLFW_KEY_C))
	{    // tell GLFW to capture our mouse
		switch (glfwGetInputMode(window, GLFW_CURSOR))
		{
		case GLFW_CURSOR_NORMAL:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case GLFW_CURSOR_DISABLED:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		}
	}
	camera->updateCameraVectors();

}
inline void DrawGUI(GLFWwindow* window)
{
	ImGui::Begin("Settings");
	ImGui::SliderFloat("ShadowResX", &shadowRes.x, 512, 8192);
	ImGui::SliderFloat("ShadowResY", &shadowRes.y, 512, 8192);
	ImGui::SliderFloat("DRLOffest", &DRLOffest, -256, 256);
	ImGui::SliderFloat("DRLOffest2", &DRLOffest2, -256, 256);
	ImGui::Checkbox("Screen Space Shadows", &sspp.at(0)->enabled);
	ImGui::Checkbox(" backCull", &backCull);
	ImGui::Checkbox(" Move", &Move);
	ImGui::Checkbox("Draw Occlusion Boxes", &DrawOcclusionBoxes);
	bool ttt = DSided; 
	ImGui::Checkbox(" DSided", &DSided);
	if (ttt != DSided)
	{
		glEnable(GL_CULL_FACE);
		if(DSided)glCullFace(GL_FRONT);
		else glCullFace(GL_BACK);
	}
	ImGui::End();
#ifdef Profiler
	ImGui::Begin("Profiler");
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	ImGui::BulletText(std::format("CPU: {} ms", CPUTime).c_str());
	ImGui::BulletText(std::format("GPU: {} ms", GPUTime).c_str());
	ImGui::BulletText(std::format("Full: {} ms", dt/sec2ms).c_str());
	ImGui::BulletText(std::format("Mem: {} mb",round( virtualMemUsedByMe* bit2mb*100.f)/100.f).c_str());
	ImGui::BulletText(std::format("avg.Fps: {} avg.ms: {}",frameRate, averageFrameTimeMilliseconds).c_str());
	ImGui::BulletText(std::format("V: {}, T: {} ", VerCount,TrisCount).c_str());
	ImGui::End();
#endif
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
inline void ReleaseAll(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
#pragma region CALLBACKS
inline void MouseScrollCallback(GLFWwindow* window, GLdouble x, GLdouble y)
{
	y /= 10;
	precision += 1 * dt * y;
	if (Input->IsKeyHold(GLFW_KEY_SPACE))Quadratic += y;
	else Linear += y;

}
inline void MousePositionCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
		camera->ProcessMouseMovement(xoffset, yoffset);
}
inline void OnWindowResize(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	projection = glm::perspective(glm::radians(70.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.01f, 100.0f);
	IsProjectionDirty = true;
	CameraDepth.Resize(width, height);
	//glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	//glBindTexture(GL_TEXTURE_2D, gPosition);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	////normal color buffer
	//glBindTexture(GL_TEXTURE_2D, gNormal);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	////color + specular color buffer
	//glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

	glViewport(0, 0, width, height);
}
#pragma endregion

#pragma endregion






//std::cout << "v=================   For loop Test   =================v" << "\n\n";      start = std::chrono::high_resolution_clock::now();
//long c = 2147483640;
//long a = 0;
//for (long i = 0; i < c; i++)
//{
//	{
//		a++;
//		if (a == 2147483640)a = -1;
//	}
//}
//LogTotalTime(start);
//std::cout << a << "^=================   For loop Test   =================^" << "\n";