#include "Font.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace gui;
using namespace std;
using namespace glm;

Font::Font() {
    fontmap = load_tga("res/fonts/helvetica.tga", 0, false);
    load_glyphs("res/fonts/helvetica.fnt");
}

Font::~Font() {
    delete_texture(fontmap);
}


Glyph Font::get_glyph(char c) {
    return glyphs[c];
}

void Font::upload_texture(GLuint shader, std::string sampler) {
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, fontmap.id);
    glUniform1i(glGetUniformLocation(shader, sampler.c_str()), 0);
}

string strip(string in) {
    return in.substr(in.find("=",0)+1, in.length());
}

void Font::load_glyphs(std::string loc) {

    ifstream file(loc);
    if (!file.is_open())
    {
        throw std::runtime_error(std::string("Failed to open file: ") + loc);
    }
	float psize = 32.0f;
    string line;
    string token, word;
    int c;
	getline(file, line);
	stringstream ss(line);
	while(token.find("size") == string::npos) ss >> token;
	stringstream s(strip(token));
	s >> psize;


    while (getline(file, line)) {
        stringstream ss(line);
        ss >> token;
        if (token == "char") {
            Glyph g;

            // Read character id
            ss >> word;
            stringstream id(strip(word));
            id >> c;

            // Read positions
            ss >> word;
            stringstream xp(strip(word));
            xp >> g.uv_pos.x;

            ss >> word;
            stringstream yp(strip(word));
            yp >> g.uv_pos.y;

            // Read dimensions
            ss >> word;
            stringstream w(strip(word));
            w >> g.uv_dim.x;

            ss >> word;
            stringstream h(strip(word));
            h >> g.uv_dim.y;

            ss >> word;
            stringstream ox(strip(word));
            ox >> g.offset.x;

            ss >> word;
            stringstream oy(strip(word));
            oy >> g.offset.y;

            ss >> word;
            stringstream a(strip(word));
            a >> g.advance;


            vec2 size(fontmap.width, fontmap.height);
            g.uv_pos = g.uv_pos / size;
            g.size = g.uv_dim / psize;
            g.uv_dim = g.uv_dim / size;
            g.offset = g.offset * (1.0f / psize);
			g.advance /= psize;
            glyphs[(char)c] = g;
        }
    }
    file.close();
}