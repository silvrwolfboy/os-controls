#include "GLTexture2Params.h"

#include <stdexcept>

GLTexture2Params::GLTexture2Params (GLsizei width, GLsizei height, GLenum internal_format)
  :
  m_target(DEFAULT_TARGET),
  m_internal_format(internal_format)
{ 
  m_size[0] = width;
  m_size[1] = height;
}

GLfloat GLTexture2Params::TexParameterf (GLenum pname) const {
  // TODO: validate that pname is a valid argument for this function (see docs of glTexParameteri/f)
  GLTexParameterfMap::const_iterator it = m_tex_parameter_f.find(pname);
  if (it == m_tex_parameter_f.end()) {
    throw std::domain_error("specified GLfloat-valued texture parameter not found and/or specified");
  }
  return it->second;
}

GLint GLTexture2Params::TexParameteri (GLenum pname) const {
  // TODO: validate that pname is a valid argument for this function (see docs of glTexParameteri/f)
  GLTexParameteriMap::const_iterator it = m_tex_parameter_i.find(pname);
  if (it == m_tex_parameter_i.end()) {
    throw std::domain_error("specified GLint-valued texture parameter not found and/or specified");
  }
  return it->second;
}

void GLTexture2Params::SetTexParameterf (GLenum pname, GLfloat value) {
  // TODO: validate that pname is a valid argument for this function (see docs of glTexParameteri/f)
  m_tex_parameter_f[pname] = value;
}

void GLTexture2Params::SetTexParameteri (GLenum pname, GLint value) {
  // TODO: validate that pname is a valid argument for this function (see docs of glTexParameteri/f)
  m_tex_parameter_i[pname] = value;
}

