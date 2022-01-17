#include "arcball_camera.h"

ArcballCamera::ArcballCamera() = default;

ArcballCamera::ArcballCamera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector, float yaw, float pitch) :
    m_eye(std::move(eye)),
    m_lookAt(std::move(lookAt)),
    m_upVector(std::move(upVector)),
    yaw(yaw),
    pitch(pitch)
{
    this->upVector_tilt = upVector;
    this->world_up = upVector;
    this->front_tilt = m_lookAt;
    this->pos = m_eye;

    this->current_lookAt = m_lookAt;

    this->zoom = 45.0f;
    this->mouse_sensitivity = 0.1f;
    this->movement_speed_table = {0.00301973834223185, 0.008356641661610944, 0.01256503263442188, 0.033824826987196674, 0.03513840852447328, 0.03650828023675758, 0.03793707066926023, 0.03942754065184267, 0.04098259041179555, 0.04260526709243335, 0.044298772701938396, 
0.04606647251843273, 0.047911903978904595, 0.04983878608137199, 0.05185102933154114, 0.053952746267215274, 0.05614826259584308, 0.05844212898287047, 0.06083913353098941, 0.06334431499296932, 0.06596297676352475, 
0.0687007016986285, 0.07156336781383614, 0.07455716491655903, 0.07768861223082595, 0.08096457707692152, 0.08439229467240426, 0.0879793891254033, 0.09173389569579381, 0.09566428440487884, 0.09977948507957878, 
0.10408891392288265, 0.10860250170846475, 0.11333072370395483, 0.11828463143439413, 0.12347588640494961, 0.12891679591003577, 0.13462035106463285, 0.1406002672028564, 0.1468710267987431, 0.15344792507485086, 
0.16034711847565264, 0.16758567619490955, 0.17518163495928912, 0.18315405728452208, 0.19152309343542892, 0.20031004733728283, 0.2095374467032763, 0.2192291176614329, 0.2294102641842239, 0.240107552645545, 
0.2513492018526619, 0.2631650789253937, 0.2755868014212743, 0.2886478461338785, 0.30238366502204944, 0.31683180876060246, 0.3320320584383634, 0.348026565967342, 0.36486000380761463, 0.382579724656366, 
0.40123593179670164, 0.4208818608526025, 0.44157397375100216, 0.4633721657507365, 0.4863399864613761, 0.5105448758430716, 0.5360584162518776, 0.5629566016740261, 0.5913201253777159, 0.6212346873026874, 
0.652791322606666, 0.6860867528942767, 0.7212237617688868, 0.7583115964716535, 0.7974663975056385, 0.8388116582869335, 0.8824787170202074, 0.928607283163904, 0.9773460010313814, 1.0288530532698925, 
1.083296807170409, 1.140856506989415, 1.2017230157101495, 1.2660996099370474, 1.3342028319048418, 1.4062634028948744, 1.4825272026873997, 1.5632563200424496, 1.6487301795951241, 1.7392467509768732, 
1.8351238464348287, 1.9367005137197573, 2.044338531552871, 2.1584240155660357, 2.2793691432427945, 2.407614007073199, 2.543628605878177, 2.6879149850642583, 2.841009537441963, 3.0034854771872954, 
};
    this->movement_speed = movement_speed_table[50];

    updateViewMatrix();

}

void ArcballCamera::printViewMatrix()
{
    printf("[%f %f %f %f]\n|%f %f %f %f|\n|%f %f %f %f|\n[%f %f %f %f]\n\n",
        m_viewMatrix[0].x, m_viewMatrix[0].y, m_viewMatrix[0].z, m_viewMatrix[0].w,
        m_viewMatrix[1].x, m_viewMatrix[1].y, m_viewMatrix[1].z, m_viewMatrix[1].w,
        m_viewMatrix[2].x, m_viewMatrix[2].y, m_viewMatrix[2].z, m_viewMatrix[2].w,
        m_viewMatrix[3].x, m_viewMatrix[3].y, m_viewMatrix[3].z, m_viewMatrix[3].w);
}

glm::mat4x4 ArcballCamera::getViewMatrix() const
{
    return m_viewMatrix;
}

glm::vec3 ArcballCamera::getEye() const
{
    return m_eye;
}

glm::vec3 ArcballCamera::getUpVector() const
{
    return m_upVector;
}

glm::vec3 ArcballCamera::getLookAt() const
{
    return m_lookAt;
}

// Camera forward axis is defined as -z
glm::vec3 ArcballCamera::getViewDirection() const
{
    return -glm::transpose(m_viewMatrix)[2];
}

glm::vec3 ArcballCamera::getRightVector() const
{
    return glm::transpose(m_viewMatrix)[0];
}

void ArcballCamera::setCameraView(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up)
{
    m_viewMatrix = glm::lookAt(eye, lookAt, up);
}

void ArcballCamera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
}

void ArcballCamera::processMouseTilt(float x_offset, float y_offset)
{

    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;


    // Constrain pitch.
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;


    updateCameraVectors();
}

void ArcballCamera::processKeyboard(Camera_Movement direction, float delta_time)
{
    float height = glm::length(m_eye - m_lookAt);
    int speed_index = floor((height - 10) * 10); // Maps height (ranging from 10 to 50) to indexes from 0 to 100.
    // std::cout << height << " " << (height - 10) * 10 << " " << speed_index << " " << movement_speed_table[speed_index] << " " << movement_speed_table.size()<< "\n";

    this->movement_speed = movement_speed_table[speed_index];

    float velocity = this->movement_speed * delta_time;

    // printf("Camera height: %f\nCamera velocity: %f\n\n", height, velocity);

    glm::vec3 new_m_eye;
    float new_height;

    if (direction == UP)
    {
        new_m_eye = m_eye + (m_eye - m_lookAt) * velocity;
        new_height = glm::length(new_m_eye - m_lookAt);

        if (new_height < 20.0)
            m_eye = new_m_eye;
    }
    if (direction == DOWN)
    {
        new_m_eye = m_eye - (m_eye - m_lookAt) * velocity;
        new_height = glm::length(new_m_eye - m_lookAt);

        if (new_height > 10.0)
            m_eye = new_m_eye;
    }

    setCameraView(m_eye, current_lookAt, m_upVector);
}

void ArcballCamera::processMouseScroll(float y_offset)
{
    zoom -= (float)y_offset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void ArcballCamera::updateCameraVectors()
{
    glm::vec3 front_vector;
    front_vector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_vector.y = sin(glm::radians(pitch));
    front_vector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


    // also re-calculate the Right and Up vector
    front_tilt = glm::normalize(front_vector);

    right_tilt = glm::normalize(glm::cross(front_tilt, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    upVector_tilt = glm::normalize(glm::cross(right_tilt, front_tilt));

    current_lookAt = -front_tilt + m_eye;

    setCameraView(m_eye, current_lookAt, m_upVector);

}

void ArcballCamera::processMouseRotation(float x_offset, float y_offset, int viewportWidth, int viewportHeight)
{

    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    glm::vec4 position(m_eye.x, m_eye.y, m_eye.z, 1.0f);
    glm::vec4 pivot(m_lookAt.x, m_lookAt.y, m_lookAt.z, 1.0f);

    float delta_angle_x = 2 * M_PI / viewportWidth; // Movement from left to right.
    float delta_angle_y = M_PI / viewportHeight;    // Moevement from top to botom.
    float x_angle = x_offset * delta_angle_x;
    float y_angle = y_offset * delta_angle_y;

    // Handle situation, when direction is the same as up vector (constaints movement up).
    float cos_angle = glm::dot(getViewDirection(), m_upVector);
    if (cos_angle * glm::sign(delta_angle_y) > 0.99f)
        delta_angle_y = 0;

    // Rotate camera around the pivot point on x axis.
    glm::mat4x4 rotation_matrix_x(1.0f);
    rotation_matrix_x = glm::rotate(rotation_matrix_x, x_angle, m_upVector);
    position = rotation_matrix_x * (position - pivot) + pivot;

    // Rotate camera around the pivot point on x axis.
    glm::mat4x4 rotation_matrix_y(1.0f);
    rotation_matrix_y = glm::rotate(rotation_matrix_y, y_angle, getRightVector());

    glm::vec3 final_position = rotation_matrix_y * (position - pivot) + pivot;
    // this->pos = final_position;

    m_eye = std::move(final_position);
    m_lookAt = std::move(getLookAt());
    m_upVector = std::move(m_upVector);

    current_lookAt = m_lookAt;

    setCameraView(m_eye, current_lookAt, upVector_tilt);
    // printViewMatrix();
}