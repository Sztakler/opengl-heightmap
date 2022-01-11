#include "sphere.h"

Sphere::Sphere(float radius, int n_sectors, int n_stacks) : 
    radius(radius), n_sectors(n_sectors), n_stacks(n_stacks)
{
    generate_vertices();
    generate_indices();

    this->vertices_array = VAO();
    this->vertices_array.Bind();
    this->vertices_buffer = VBO(&this->vertices, this->vertices.size() * sizeof(float));

    this->indices_buffer = EBO(&this->indices, this->indices.size() * sizeof(int));
    this->indices_buffer.Bind();

    this->shader = Shader("shaders/globe.vert", "shaders/globe.frag");

    this->vertices_array.link_vbo(this->vertices_buffer, 0, 3);

    this->position = {0.0, 0.0, 0.0};
}

void Sphere::Bind()
{
    this->vertices_array.Bind();
}

void Sphere::Unbind()
{
    this->vertices_array.Unbind();
}

void Sphere::Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection, DRAWING_MODE drawing_mode)
{
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(*model));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(*projection));
    glUniform3fv(3, 1, glm::value_ptr(position));

    vertices_array.Bind();
    indices_buffer.Bind();

     switch (drawing_mode)
    {
        case TRIANGLES:
             glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, (void*)0);
            break;

        case WIREFRAME:
             glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)0);
            break;
        default:
             glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, (void*)0);
    }
}

void Sphere::generate_vertices()
{
    float x, y, z;
    float sector_angle = 0;
    float stack_angle = 0;

    float rcos;

    float sector_step = 2 * M_PI / n_sectors;
    float stack_step = M_PI / n_stacks;

    for (int i = 0; i <= n_stacks; i++)
    {
        stack_angle = M_PI / 2 - i * stack_step;
        rcos = radius * cos(stack_angle);
        z = radius * sin(stack_angle);

        for (int j = 0; j <= n_sectors; j++)
        {
            sector_angle = j * sector_step;

            x = rcos * cos(sector_angle);
            y = rcos * sin(sector_angle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
}

void Sphere::generate_indices()
{
    int k1, k2;
    for (int i = 0; i < n_stacks; ++i)
    {
        k1 = i * (n_sectors + 1); // beginning of current stack
        k2 = k1 + n_sectors + 1;  // beginning of next stack

        for (int j = 0; j < n_sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
        }
    }
}