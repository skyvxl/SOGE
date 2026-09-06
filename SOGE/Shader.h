#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
 public:
  unsigned int ID;

  Shader(const char* vertexPath, const char* fragmentPath);
  void Use();
  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;

 private:
  void checkCompileErrors(unsigned int shader, std::string type);
};
