#include <STBImage/stb_image.h>
#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "terrain.h"
#include "utils.h"
#include "vertex.h"
#include "logger.h"

Terrain::Terrain(std::string path){
    // Convert '\ ' to '/' for Windows
    std::replace(path.begin(), path.end(), '\\', '/');
    
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load((path+"/heightmap.png").c_str(), &width, &height, &nrChannels, 0);

    
    int rez = 1;
    unsigned bytePerPixel = nrChannels;
    for (int i = 0; i < height; i++)
    {
        std::vector<float> temp;
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
            vertices.push_back((int)y * yScale - yShift);   // vy
            vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
            vertices.push_back((float)i);
            vertices.push_back((float)j);
            temp.push_back((int)y * yScale - yShift);
        }
        Point.push_back(temp);
    }
    stbi_image_free(data);

    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }
    
    numStrips = (height - 1) / rez;
    numTrisPerStrip = (width / rez) * 2 - 2;

    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &terrainVAO);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(terrainVAO);

    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &terrainVBO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(0);
    // texCoord attribute
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    OPENGL_EXTRA_FUNCTIONS->glEnableVertexAttribArray(1);

    OPENGL_EXTRA_FUNCTIONS->glGenBuffers(1, &terrainIBO);
    OPENGL_EXTRA_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    OPENGL_EXTRA_FUNCTIONS->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    
    //textureID = loadTexture2(texName, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    tex = loadTexture(path + "/texture.jpg");

}

unsigned int Terrain::loadTexture(std::string path) {
    unsigned int textureID;
    OPENGL_FUNCTIONS->glGenTextures(1, &textureID);
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, textureID);

    

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }
        else {
            return 0;
        }

        OPENGL_FUNCTIONS->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        OPENGL_FUNCTIONS->glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    return textureID;
}

void Terrain::render() {
    // Set the texture wrapping parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE2);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, tex);

    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(terrainVAO);
    for (unsigned strip = 0; strip < numStrips; strip++)
    {
        OPENGL_EXTRA_FUNCTIONS->glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            numTrisPerStrip + 2,   // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index

    }
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    OPENGL_EXTRA_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, 0);
}

float Terrain::GetHeight(float px, float pz) {
    float fx = px + double(height) / 2;
    float fz = pz + double(width) / 2;

    int x = ((int)fx) % height;
    int z = ((int)fz) % width;
    int gx = (x + 1) % height;
    int gz = (z + 1) % width;

    // prevent retrieving out of bounds
    if (x < 0) x = 0;
    if (z < 0) z = 0;
    if (gx < 0) gx = 0;
    if (gz < 0) gz = 0;
    if (x > height - 1) x = height - 1;
    if (z > width - 1) z = width - 1;
    if (gx > height - 1) gx = height - 1;
    if (gz > width - 1) gz = width - 1;
    float ans = (x - fx) * (Point[gx][gz] - Point[x][z]) + Point[x][z];

    //float ans = Point[x][z];

    return ans;
}

glm::vec3 Terrain::GetNormal(glm::vec3 pos) {
    // construct a triangle with its geometry center at pos
    glm::vec3 p1 = pos + glm::vec3(-1.0f, 0.0f, -0.57735f);
    glm::vec3 p2 = pos + glm::vec3(1.0f, 0.0f, -0.57735f);
    glm::vec3 p3 = pos + glm::vec3(0.0f, 0.0f, 1.1547f);
    
    // calculate the height
    p1.y = GetHeight(p1.x, p1.z);
    p2.y = GetHeight(p2.x, p2.z);
    p3.y = GetHeight(p3.x, p3.z);

    // calculate the normal
    glm::vec3 v1 = p2 - p1;
    glm::vec3 v2 = p3 - p1;
    glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    // make the normal point up
    if (normal.y < 0.0f) {
        normal = -normal;
    }

    //glm::vec3 point1(fx - 1, GetHeight(fx - 1, fz - 1), fz - 1);
    //glm::vec3 point2(fx + 1, GetHeight(fx + 1, fz + 1), fz + 1);

    //glm::vec3 l1 = pos - point1;
    //glm::vec3 l2 = point2 - point1;
    
    //glm::vec3 ans = glm::normalize(glm::cross(l1, l2));
    return normal;
}

HitRecord Terrain::hit(const Ray& ray) {
    glm::vec3 orig = ray.origin();
    glm::vec3 dir = ray.direction();
    if (orig.x >= width / 2 || orig.x <= -width / 2 || orig.z >= height / 2 || orig.z <= -height / 2) {
        return HitRecord();
    }

    // A good ray step is half of the blockScale 
    glm::vec3 rayStep = dir;
    glm::vec3 rayStartPosition = orig;

    // Linear search - Loop until find a point inside and outside the terrain Vector3 
    glm::vec3 lastRayPosition = orig; 
    orig += rayStep;
    float map_height = GetHeight(orig.x,orig.z);
    while (orig.y > map_height)
    {
        lastRayPosition = orig;
        orig += rayStep;
        if (orig.x >= width / 2 || orig.x <= -width / 2 || orig.z >= height / 2 || orig.z <= -height / 2) {
            return HitRecord();
        }
        map_height = GetHeight(orig.x, orig.z);
    }

    glm::vec3 startPosition = lastRayPosition;
    glm::vec3 endPosition = orig;

    // Binary search with 64 steps. Try to find the exact collision point 
    float threshold = 0.1f;
    glm::vec3 mid;
    while(true) {
        mid = (startPosition + endPosition) * 0.5f;
        map_height = GetHeight(mid.x, mid.z);
        if (abs(mid.y - map_height) < threshold) {
            break;
        }
        else if (mid.y - endPosition.y < 0.001f) {
            // if no more space to search, return
            break;
        }
        Logger::debug("Current height difference: " + std::to_string(abs(mid.y - map_height)));
        if (mid.y > map_height)
        {
            startPosition = mid;
        }
        else
        {
            endPosition = mid;
        }
    }
    //{
    //    // Binary search pass 
    //    glm::vec3 middlePoint = (startPosition + endPosition) * 0.5f;
    //    if (middlePoint.y < height)
    //        endPosition = middlePoint;
    //    else
    //        startPosition = middlePoint;
    //}
    glm::vec3 position = mid;
    glm::vec3 normal = GetNormal(position);

    // If t > 200, consider the ray as not hitted
    float t = glm::length(position - rayStartPosition);
    if (t > 200.0f)
        return HitRecord();
    else
        return HitRecord(t, position, normal);
}