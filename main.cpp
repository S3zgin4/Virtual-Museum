#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdio>
#include "Model.h"
#include "Pedestal.h"

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "Robot.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


unsigned int width = 800;
unsigned int height = 800;
int windowWidth = 800;
int windowHeight = 800;

// Global variables for UI
static int selected_statue = 0;

// Information about each model in the museum
struct ModelInfo {
    std::string name;
    std::string description;
};

// Model information for each statue
std::vector<ModelInfo> modelInfos = {
    { "Erkek Heykeli\nStatue of a Man",
      "TURKCE\n"
      "Tarih/Donem: Roma Donemi, M.S. 2. yuzyil\n"
      "Malzeme    : Bronz\n"
      "Aciklama   : Togali bu bronz heykelin Romalilar zamaninda Magarsus antik kentine gelen,yönetimde söz sahibi bir senatöre ait oldugu tahmin edilmektedir.\n\n"
      "ENGLISH\n"
      "Date/Period: Roman Period, 2nd century AD\n"
      "Material   : Bronze\n"
      "Description: This bronze statue depicting a man in a toga is believed to representa senator or high-ranking official who visited the ancient city ofMagarsus during the Roman period."
    }
,

    { "Lahit - Aslanli (Govdeye Islenmis Aslan Figurleri)\nSarcophagus with Lion Reliefs",
      "TURKCE\n"
      "Tarih/Donem: Roma Donemi, M.S. 2. yuzyil\n"
      "Malzeme    : Mermer\n"
      "Aciklama   : Bu anitsal lahdin uzun yuzeyinde yer alan aslan kabartmalari, olu sahibinin gucunu ve koruyuculugunu simgeler. Lahitler, yuksek statulu bireylerin olu gomme geleneginde onemli yer tutar.\n\n"
      "ENGLISH\n"
      "Date/Period: Roman Period, 2nd century AD\n"
      "Material    : Marble\n"
      "Description : The long side of this monumental sarcophagus features reliefs of lions,symbolizing the strength and protective power of the deceased. Sarcophagilike this were significant in the burial traditions of high-status individuals during the Roman era."
    },

    { "Arabali Tanri Tarhunda (Firtina Tanrisi)\nChariot-Borne God Tarhunda (Storm God)",
      "TURKCE\n"
      "Tarih/Donem: Gec Hitit Donemi, M.O. 9. yuzyil\n"
      "Malzeme    : Bazalt tas\n"
      "Aciklama   : Gec Hitit donemine ait bir firtina tanrisidir. Adana'nin Yuregir ilcesi,Cine Koyu'nde bir tarlada bulunmus ve Adana Muzesi'nde sergilenmektedir.Tanri, iki boga tarafindan cekilen bir araba icinde tasvir edilmistir. Arabadaki figur, elinde yildirim demeti tasimakta olup, gokyuzune hukmeden gucleri simgeler.\n\n"
      "ENGLISH\n"
      "Date/Period: Late Hittite Period, 9th century BC\n"
      "Material    : Basalt stone\n"
      "Description : This figure of the storm god Tarhunda originates from the Late Hittite period and was discovered in a field in Cine Village, Yuregir district of Adana. The god is depicted riding a chariot drawn by two bulls, holding a bundle of lightning bolts in his hand."
    },

    { "Lahit - Figurlu (Mitolojik Sahne Tasviri)\nSarcophagus with Mythological Figures",
      "TURKCE\n"
      "Tarih/Donem: Roma Donemi, M.S. 2.-3. yuzyil\n"
      "Malzeme    : Mermer\n"
      "Aciklama   : Bu lahit uzerinde mitolojik sahneler ve figurler yer alir. Olumden sonraki yasami simgeleyen bu sahneler, Roma'nin dinsel ve kulturel inanclarini yansitir. Ayrintili iscilik, donemin sanat anlayisini gosterir.\n\n"
      "ENGLISH\n"
      "Date/Period: Roman Period, 2nd-3rd century AD\n"
      "Material    : Marble\n"
      "Description : This sarcophagus is adorned with mythological scenes and figures, representing beliefs in the afterlife. The detailed craftsmanship reflects Roman religious and cultural values, as well as the artistic style of the period."
    },

    { "Kadin Rolyefli Mezar Steli\nFunerary Stele with Female Relief",
      "TURKCE\n"
      "Tarih/Donem: Roma Donemi, M.S. 2.- 3. yuzyil\n"
      "Malzeme    : Kirectasi\n"
      "Aciklama   : Ust kismi ucgen alinlikli bu mezar tasi, olen kadinin portresini icerir. Stelin uzerindeki yazi ve suslemeler, hem kimlik bilgisini hem de sosyal statusu ifade eder. Anadolu'da sikca gorulen mezar aniti tipindedir.\n\n"
      "ENGLISH\n"
      "Date/Period: Roman Period, 2nd-3rd century AD\n"
      "Material    : Limestone\n"
      "Description : This funerary stele, crowned with a triangular pediment, bears a portrait of the deceased woman. The inscriptions and decorative elements on the stele indicate both her identity and social status. It is a common type of grave monument found across Anatolia."
    }
};
// Variables for popup display
static bool showModelPopup = false;
static int currentPopupModel = -1;
static bool waitingForUserInput = false; // New flag to indicate we're waiting for user to click "Next"

// Lighting control variables
static float ambientStrength = 0.3f;    // Default ambient light strength
static glm::vec3 lightColor(1.0f, 1.0f, 0.9f);  // Default light color
static glm::vec3 lightPos(0.0f, 4.0f, 0.0f);    // Default light position
static bool lightingControlOpen = true;  // Keep the lighting panel open by default

// Scanning popup variables
static bool showScanningPopup = false;
static float scanningProgress = 0.0f;
static float scanningSpeed = 1.0f; // Saniyede yüzde olarak ilerleme hızı
static int scanningModelIndex = -1;

// Manual scanning variables
static bool manualScanningActive = false;
static float manualScanTimer = 0.0f;
static float scanningDelay = 2.0f; // Robot heykelin önünde 2 saniye durduktan sonra tarama başlasın
static int nearestModelIndex = -1;
static float nearModelDistance = 5.0f; // Bu mesafe içindeyse heykele "yakın" kabul edilecek

// Scanning state variables
static std::vector<bool> modelScanned(5, false); // 5 heykelin her biri için tarama durumu
static float resetDistance = 8.0f; // Bu mesafeden daha uzağa gidince tarama durumu sıfırlanacak

// Intro popup variable
static bool showIntroPopup = true; // Will show the intro popup when the app starts

// Callback function to adjust the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

// Process keyboard input
void processInput(GLFWwindow* window, Robot& robot, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 pos = robot.getPosition();
    float speed = 2.0f * deltaTime;
    
    bool moved = false;
    
    // Ok tuşlarıyla normal hareket - dünya koordinatlarına göre
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        pos.z -= speed; // İleri hareket (negatif Z yönü)
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pos.z += speed; // Geri hareket (pozitif Z yönü)
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        pos.x -= speed; // Sol hareket (negatif X yönü)
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        pos.x += speed; // Sağ hareket (pozitif X yönü)
        moved = true;
    }
    
    // Eğer hareket ettiysek
    if (moved) {
        // Robotu hareket ettir
        robot.setPosition(pos);
        
        // Robotun rotasyonunu zorla 180 dereceye ayarla
        robot.rotateBy(180.0f - robot.getCurrentRotation());
        
        // Hareket yönünde bir hedef nokta belirle - rotasyon gözardı edilecek
        glm::vec3 targetPoint = pos + glm::vec3(0.0f, 0.0f, -5.0f); // Her zaman -Z yönüne doğru
        
        // Robotun kolunu hedef noktaya doğru döndür
        robot.pointArmTo(targetPoint);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Configure GLFW: OpenGL version 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, "Virtual Museum Assignment", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set callback for window resize events
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set the viewport dimensions
    glViewport(0, 0, width, height);

    //a cube with positions, colors, texture coordinates, texture flag(1.0=texture, 0.0=no-texture)
    float vertices[] = {
        //front(z = -15.0)
        -15.0f, -5.0f, -15.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,   1.0f,  // position, normal, texcoord, useTexture
         15.0f, -5.0f, -15.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,   1.0f,
         15.0f,  5.0f, -15.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,   1.0f,
        -15.0f,  5.0f, -15.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,   1.0f,

        //back(z = 15.0)
        -15.0f, -5.0f,  15.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,   1.0f,
         15.0f, -5.0f,  15.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,   1.0f,
         15.0f,  5.0f,  15.0f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f,   1.0f,
        -15.0f,  5.0f,  15.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,   1.0f,

        //left(x = -15.0)
        -15.0f, -5.0f, -15.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.0f,
        -15.0f, -5.0f,  15.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   1.0f,
        -15.0f,  5.0f,  15.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.0f,
        -15.0f,  5.0f, -15.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   1.0f,

        //right(x = 15.0)
        15.0f, -5.0f, -15.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,   1.0f,
        15.0f, -5.0f,  15.0f,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f,   1.0f,
        15.0f,  5.0f,  15.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   1.0f,
        15.0f,  5.0f, -15.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,   1.0f,

        //floor(y = -5)
        -15.0f, -5.0f, -15.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,   0.0f,
         15.0f, -5.0f, -15.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   0.0f,
         15.0f, -5.0f,  15.0f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,   0.0f,
        -15.0f, -5.0f,  15.0f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,   0.0f,

        //ceiling(y = 5)
        -15.0f,  5.0f, -15.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,   0.0f,
         15.0f,  5.0f, -15.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,   0.0f,
         15.0f,  5.0f,  15.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,   0.0f,
        -15.0f,  5.0f,  15.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,   0.0f
    };

    unsigned int indices[] = {
        //front
        0, 3, 2,
        2, 1, 0,

        //back
        4, 5, 6,
        6, 7, 4,

        //left
        8, 9, 10,
        10, 11, 8,

        //right
        12, 15, 14,
        14, 13, 12,

        //floor
        16, 17, 18,
        18, 19, 16,

        //ceiling
        20, 23, 22,
        22, 21, 20
    };

    // Enable depth testing and disable face culling
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Initialize shaders
    Shader roomShader("vertexShader.glsl", "fragmentShader.glsl");
    Shader robotShader("vertexShader.glsl", "fragmentShader.glsl");

    // Initialize room VAO, VBO, EBO
    VAO VAO1;
    VAO1.Bind();
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 1, GL_FLOAT, 9 * sizeof(float), (void*)(8 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Initialize wall texture
    Texture wall("wall_orijinal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    glUniform1i(glGetUniformLocation(roomShader.ID, "tex0"), 0);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 10.0f));
    camera.Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.Up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Initialize robot
    Robot robot;
    robot.setScale(glm::vec3(1.0f));
    // Robotu toplam 180 derece sağa döndür (90 + 90 derece)
    robot.rotateBy(180.0f);

    // Initialize models and pedestals
    std::vector<std::unique_ptr<Model>> models;
    std::vector<std::unique_ptr<Pedestal>> pedestals;

    // Create pedestals at different positions along the walls
    glm::vec3 pedestalPositions[] = {
        glm::vec3(-13.0f, -5.0f, -13.0f),  // Sol ön köşe
        glm::vec3(-13.0f, -5.0f, 0.0f),    // Sol orta
        glm::vec3(-13.0f, -5.0f, 13.0f),   // Sol arka köşe
        glm::vec3(13.0f, -5.0f, -13.0f),   // Sağ ön köşe
        glm::vec3(13.0f, -5.0f, 13.0f)     // Sağ arka köşe
    };

    // Create all pedestals
    std::cout << "Creating pedestals..." << std::endl;
    for (size_t i = 0; i < 5; i++) {
        try {
            auto pedestal = std::make_unique<Pedestal>();
            pedestal->setPosition(pedestalPositions[i]);
            pedestal->setScale(glm::vec3(2.5f, 1.0f, 2.5f));
            pedestal->setColor(glm::vec3(0.4f, 0.4f, 0.4f));
            pedestals.push_back(std::move(pedestal));
            std::cout << "Created pedestal " << (i+1) << " at position (" 
                     << pedestalPositions[i].x << ", " 
                     << pedestalPositions[i].y << ", " 
                     << pedestalPositions[i].z << ")" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error creating pedestal " << (i+1) << ": " << e.what() << std::endl;
        }
    }
    std::cout << "Total pedestals created: " << pedestals.size() << std::endl;

    // Model yükleme konfigürasyonları
    struct ModelConfig {
        const char* path;
        float scale;
        float height;
        float rotation;
    };

    ModelConfig modelConfigs[] = {
        {"Models/model1.obj", 0.4f, -0.5f, 50.0f},   // Sol ön - Rotasyon 135 derece yapıldı (merkeze bakacak şekilde)
        {"Models/model2.obj", 0.9f, -0.1f, -90.0f},   // Sol orta
        {"Models/model3.obj", 0.4f, -0.35f, 135.0f},  // Sol arka
        {"Models/model4.obj", 0.7f, -0.5f, 45.0f},    // Sağ ön
        {"Models/model5.obj", 0.8f, -0.5f, 45.0f}     // Sağ arka
    };

    // Load all models with error handling
    std::cout << "\nLoading models..." << std::endl;
    for (size_t i = 0; i < 5; i++) {
        try {
            std::cout << "Attempting to load model " << (i+1) << " from " << modelConfigs[i].path << std::endl;
            models.push_back(std::make_unique<Model>(modelConfigs[i].path));
            
            glm::vec3 pedestalTop = pedestals[i]->getTopPosition();
            pedestalTop.y += modelConfigs[i].height;
            models.back()->setPosition(pedestalTop);
            models.back()->setScale(glm::vec3(modelConfigs[i].scale));
            models.back()->setRotation(glm::vec3(0.0f, modelConfigs[i].rotation, 0.0f));
            
            std::cout << "Model " << (i+1) << " loaded and configured successfully" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading model " << (i+1) << ": " << e.what() << std::endl;
        }
    }
    std::cout << "Total models loaded: " << models.size() << std::endl;

    float lastFrame = 0.0f;

    // İmGui için gereken değişkenler (global kapsamda tanımlayalım)
    static bool show_popup = true;

	//ImGui initialization
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

    // UTF-8 desteği için font yapılandırması
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arial.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    // veya şu şekilde kullanabilirsiniz (projenizin dizininde bir font varsa):
    // io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    // Türkçe karakter desteği için yapılandırma
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Klavye navigasyonu için
    io.IniFilename = NULL; // INI dosyası kullanma

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


    glfwSwapBuffers(window);
    // Main render loop
    static glm::vec3 robotStartPos = robot.getPosition(); // Robot başlangıç pozisyonu  
    static glm::vec3 roomCenter(0.0f, 0.0f, 0.0f); // Odanın merkezi
    static bool tourActive = false;
    static bool tourCompleted = false;
    static bool returningToCenter = false; // Merkeze dönüş durumu
    static bool visitingModel = false; // Modeli ziyaret etme durumu

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window, robot, deltaTime);

        // Clear the screen
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera
        camera.Inputs(window);

        // Robot'un otomatik turu için işlem ekleyelim
        if (tourActive && robot.isCurrentlyTouring() && !robot.isCurrentlyMoving()) {
            int currentModelIndex = robot.getCurrentModelIndex();
            
            // Yer seviyesi hesapla
            float groundY = -5.0f;
            float legHeight = 0.4f;
            float bodyHeight = 0.4f;
            float robotGroundY = groundY + legHeight + bodyHeight * 0.5f;
            
            // Merkezdeki pozisyon
            glm::vec3 centerPos = roomCenter;
            centerPos.y = robotGroundY;
            
            if (returningToCenter) {
                // Merkeze dönüş tamamlandı, sonraki modele geçiş
                returningToCenter = false;
                visitingModel = false;
                waitingForUserInput = false;
                
                // Sonraki modele geç
                if (currentModelIndex + 1 < models.size()) {
                    // Tur sırasında her zaman tarama yapıyoruz, bu yüzden taranmamış olarak işaretle
                    modelScanned[currentModelIndex + 1] = false;
                    robot.jumpToModel(currentModelIndex + 1, models.size());
                    std::cout << "Next model: " << (currentModelIndex + 2) << std::endl;
                } else {
                    // Tüm modeller gezildi
                    robot.pauseTour();
                    tourActive = false;
                    tourCompleted = true;
                    std::cout << "Tour completed!" << std::endl;
                }
            }
            else if (visitingModel) {
                // Show scanning popup first, then model info popup when scanning is complete
                if (!showScanningPopup && !showModelPopup && currentModelIndex >= 0 && currentModelIndex < modelInfos.size()) {
                    showScanningPopup = true;
                    scanningProgress = 0.0f;
                    scanningModelIndex = currentModelIndex;
                    std::cout << "Scanning model " << (currentModelIndex + 1) << std::endl;
                }
                
                // Wait for scanning to complete before showing model info
                if (showScanningPopup && scanningProgress >= 100.0f) {
                    showScanningPopup = false;
                    showModelPopup = true;
                    waitingForUserInput = true;
                    currentPopupModel = scanningModelIndex;
                    std::cout << "Showing popup for model " << (scanningModelIndex + 1) << std::endl;
                }
                
                // Wait for user to click "Next" button (handled in ImGui section)
                // Robot stays at the model until user interaction
            }
            else if (currentModelIndex >= 0 && currentModelIndex < models.size()) {
                // Modele git
                selected_statue = currentModelIndex + 1;
                std::cout << "Robot moving to model: " << selected_statue << std::endl;

                glm::vec3 modelPos = models[currentModelIndex]->fetchPosition();
                glm::vec3 modelRot = models[currentModelIndex]->fetchRotation();

                float yaw = glm::radians(modelRot.y);
                glm::vec3 modelForward(-sin(yaw), 0.0f, -cos(yaw));
                glm::vec3 targetPos;

                float forwardDistance = 4.0f;
                if (currentModelIndex == 2) {
                    forwardDistance = 6.0f;
                }

                if (currentModelIndex == 1) { // Model 2 için merkeze doğru daha uzak pozisyonla
                    glm::vec3 toCenter = glm::normalize(roomCenter - modelPos);
                    targetPos = modelPos + toCenter * forwardDistance * 1.5f; // 1.5 kat daha uzak
                }
                else if (currentModelIndex == 4) { // Model 5 için merkeze doğru daha uzak
                    glm::vec3 toCenter = glm::normalize(roomCenter - modelPos);
                    targetPos = modelPos + toCenter * forwardDistance * 1.3f; // 1.3 kat daha uzak
                }
                else if (currentModelIndex == 0 || currentModelIndex == 2 || currentModelIndex == 3) {
                    glm::vec3 toStart = glm::normalize(roomCenter - modelPos);
                    targetPos = modelPos + toStart * forwardDistance;
                } else {
                    targetPos = modelPos + modelForward * forwardDistance;
                }
                
                targetPos.y = robotGroundY;
                robot.moveTo(targetPos);
                robot.pointArmTo(modelPos);
                visitingModel = true;
            }
        }

        // Robot manuel kontrolde heykel tarama işlevi
        if (!tourActive && !showScanningPopup && !showModelPopup) {
            // Robotun her bir heykele olan mesafesini kontrol et
            glm::vec3 robotPos = robot.getPosition();
            float minDistance = nearModelDistance + 1.0f;
            int closestModelIdx = -1;
            
            // En yakın heykeli bul
            for (size_t i = 0; i < models.size(); i++) {
                glm::vec3 modelPos = models[i]->fetchPosition();
                float distance = glm::length(glm::vec2(robotPos.x - modelPos.x, robotPos.z - modelPos.z));
                
                if (distance < nearModelDistance && distance < minDistance) {
                    minDistance = distance;
                    closestModelIdx = i;
                }
                
                // Heykelden uzaklaştığını kontrol et ve tarama durumunu sıfırla
                if (distance > resetDistance && modelScanned[i]) {
                    modelScanned[i] = false;
                    std::cout << "Reset scanning status for model " << (i + 1) << " (distance: " << distance << ")" << std::endl;
                }
            }
            
            // Eğer robot bir heykele yakınsa
            if (closestModelIdx >= 0) {
                // Daha önce taranmamış bir model mi?
                if (!modelScanned[closestModelIdx]) {
                    if (closestModelIdx == nearestModelIndex) {
                        manualScanTimer += deltaTime;
                        
                        // Yeterli süre yakında durulduysa taramayı başlat
                        if (manualScanTimer >= scanningDelay && !manualScanningActive) {
                            manualScanningActive = true;
                            showScanningPopup = true;
                            scanningProgress = 0.0f;
                            scanningModelIndex = closestModelIdx;
                            
                            robot.pointArmTo(models[closestModelIdx]->fetchPosition());
                            std::cout << "Manual scanning started for model " << (closestModelIdx + 1) << std::endl;
                        }
                    } else {
                        nearestModelIndex = closestModelIdx;
                        manualScanTimer = 0.0f;
                    }
                } else {
                    // Bu heykel zaten taranmış, işlem yapma
                    nearestModelIndex = closestModelIdx;
                }
            } else {
                nearestModelIndex = -1;
                manualScanTimer = 0.0f;
            }
        }

        // Manuel tarama durumunu kontrol et
        if (manualScanningActive && showScanningPopup) {
            // Update scanning progress
            scanningProgress += scanningSpeed * 100.0f * deltaTime;
            
            // Tarama tamamlandıysa bilgi penceresini göster
            if (scanningProgress >= 100.0f) {
                std::cout << "Manual scanning completed at " << scanningProgress << "%, showing info popup" << std::endl;
                showScanningPopup = false; // Tarama popup'ını kapat
                showModelPopup = true; // Model bilgi popup'ını göster
                currentPopupModel = scanningModelIndex; // Popupda gösterilecek model indeksini ayarla
                waitingForUserInput = true; // User input beklendiğini belirt
                
                // Heykelin tarandığını işaretle
                if (scanningModelIndex >= 0 && scanningModelIndex < modelScanned.size()) {
                    modelScanned[scanningModelIndex] = true;
                    std::cout << "Model " << (scanningModelIndex + 1) << " marked as scanned" << std::endl;
                }
            }
        }

        // Eğer model bilgisi kapatıldıysa manuel tarama durumunu sıfırla
        if (!showModelPopup && !showScanningPopup) {
            manualScanningActive = false;
        }

        // Render room
        roomShader.Activate();
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
            0.1f,
            100.0f
        );
        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);

        roomShader.setMat4("projection", projection);
        roomShader.setMat4("view", view);
        roomShader.setMat4("model", glm::mat4(1.0f));

        // Set lighting parameters for room
        roomShader.setVec3("lightPos", lightPos);
        roomShader.setVec3("lightColor", lightColor);
        roomShader.setVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
        roomShader.setVec3("viewPos", camera.Position);
        glUniform1f(glGetUniformLocation(roomShader.ID, "ambientStrength"), ambientStrength);

        wall.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
       
      // Render pedestals with debug info
        std::cout << "\nRendering " << pedestals.size() << " pedestals..." << std::endl;
        for (size_t i = 0; i < pedestals.size(); i++) {
            try {
                pedestals[i]->render(roomShader);
                std::cout << "Rendered pedestal " << (i + 1) << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error rendering pedestal " << (i + 1) << ": " << e.what() << std::endl;
            }
        }

        // Render models with debug info
        std::cout << "\nRendering " << models.size() << " models..." << std::endl;
        if (!models.empty()) {
            robotShader.Activate();
            robotShader.setMat4("projection", projection);
            robotShader.setMat4("view", view);
            robotShader.setVec3("lightPos", lightPos);
            robotShader.setVec3("lightColor", lightColor);
            robotShader.setVec3("objectColor", glm::vec3(0.9f, 0.8f, 0.7f));
            robotShader.setVec3("viewPos", camera.Position);
            glUniform1f(glGetUniformLocation(robotShader.ID, "ambientStrength"), ambientStrength);

            for (size_t i = 0; i < models.size(); i++) {
                try {
                    // Update rotation
                    glm::vec3 currentRotation = models[i]->fetchRotation();
                    currentRotation.y += 30.0f * deltaTime;
                    if (currentRotation.y >= 360.0f) currentRotation.y -= 360.0f;
                    models[i]->setRotation(currentRotation);

                    // Get model matrix
                    glm::mat4 modelMatrix = models[i]->getModelMatrix();
                    // Calculate normal matrix on CPU
                    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
                    // Set uniforms
                    robotShader.setMat4("model", modelMatrix);
                    robotShader.setMat3("normalMatrix", normalMatrix);
                    models[i]->render(robotShader);
                    std::cout << "Rendered model " << (i + 1) << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error rendering model " << (i + 1) << ": " << e.what() << std::endl;
                }
            }
        }

        // Render robot
        robotShader.Activate();
        robotShader.setMat4("projection", projection);
        robotShader.setMat4("view", view);
        robotShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f)); // Kırmızı robot için
        robot.update(deltaTime);
        robot.render(robotShader);

        // Render ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Ana Virtual Museum paneli
        ImGui::Begin("Virtual Museum");
        ImGui::Text("Welcome to the Virtual Museum!");
        ImGui::Text("Use arrow keys to move the robot.");
        ImGui::Text("Press ESC to exit.");
        ImGui::Text("Selected Statue: %d", selected_statue);

        if (ImGui::Button("Start Tour", ImVec2(100, 40))) {
            if (!tourActive && !robot.isCurrentlyTouring()) {
                // Robot merkeze gel
                glm::vec3 centerPos = roomCenter;
                float groundY = -5.0f;
                float legHeight = 0.4f;
                float bodyHeight = 0.4f;
                float robotGroundY = groundY + legHeight + bodyHeight * 0.5f;
                centerPos.y = robotGroundY;
                
                // Önce robot merkeze konumlandı
                robot.setPosition(centerPos);
                robot.pointArmTo(centerPos);
                
                // Turu başlat
                robot.startModelTour();
                tourActive = true;
                tourCompleted = false;
                visitingModel = false;
                returningToCenter = false;
                waitingForUserInput = false;
                showModelPopup = false;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop Tour", ImVec2(100, 40))) {
            if (tourActive || robot.isCurrentlyTouring()) {
                robot.pauseTour();
                // Merkeze dön
                glm::vec3 centerPos = roomCenter;
                float groundY = -5.0f;
                float legHeight = 0.4f;
                float bodyHeight = 0.4f;
                float robotGroundY = groundY + legHeight + bodyHeight * 0.5f;
                centerPos.y = robotGroundY;
                robot.moveTo(centerPos);
                robot.pointArmTo(centerPos);
                returningToCenter = true;
                tourActive = false;
                tourCompleted = false;
                waitingForUserInput = false;
                showModelPopup = false;
            }
        }
        if (tourCompleted) {
            ImGui::Text("Tour completed! Robot is at the center.");
        }
        ImGui::End();

        // Ayrı Işık Kontrol paneli - sağ üst köşede konumlandırılmış
        // Simplified panel with only ambient light control
        ImVec2 lightPanelSize(300, 120); // Reduced height since we're only showing ambient control
        ImGui::SetNextWindowSize(lightPanelSize);
        ImGui::SetNextWindowPos(ImVec2(windowWidth - lightPanelSize.x - 10, 10)); // Sağ üst köşeye yerleştir
        ImGui::Begin("Lighting Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

        // Title with slightly larger font
        ImGui::SetWindowFontScale(1.2f); 
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Lighting Settings");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();

        // More spacing after title
        ImGui::Spacing();
        ImGui::Spacing();

        // Only show ambient strength control
        ImGui::SetWindowFontScale(1.1f);
        ImGui::Text("Ambient Light");
        ImGui::SetWindowFontScale(1.0f);

        ImGui::PushItemWidth(lightPanelSize.x - 110);
        if (ImGui::SliderFloat("Strength##Ambient", &ambientStrength, 0.0f, 1.0f)) {
            roomShader.Activate();
            glUniform1f(glGetUniformLocation(roomShader.ID, "ambientStrength"), ambientStrength);
            
            robotShader.Activate();
            glUniform1f(glGetUniformLocation(robotShader.ID, "ambientStrength"), ambientStrength);
        }
        ImGui::PopItemWidth();

        // Description of what ambient light does
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Light gray text for description
        ImGui::TextWrapped("Adjusts the overall brightness of the scene");
        ImGui::PopStyleColor();

        ImGui::End();

        // Display model information popup with Next button
        if (showModelPopup && currentPopupModel >= 0 && currentPopupModel < modelInfos.size()) {
            // Debug çıktısı
            std::cout << "Showing model info popup for model " << (currentPopupModel + 1) 
                      << ", waitingForUserInput: " << (waitingForUserInput ? "true" : "false")
                      << ", visitingModel: " << (visitingModel ? "true" : "false")
                      << ", manualScanningActive: " << (manualScanningActive ? "true" : "false")
                      << std::endl;
                      
            // Calculate popup position (center of screen) - MORE INCREASED SIZE
            ImVec2 popupSize(650, 500); // Further increased from (600, 450)
            ImVec2 center = ImVec2(windowWidth * 0.5f, windowHeight * 0.35f); // Moved slightly down
            ImGui::SetNextWindowPos(ImVec2(center.x - popupSize.x * 0.5f, center.y - popupSize.y * 0.5f));
            ImGui::SetNextWindowSize(popupSize);
            
            // Create popup with model information
            ImGui::Begin("Model Information", NULL, 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
            
            // Title with bold, larger font - FURTHER INCREASED FONT SIZE
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));  // Gold color
            ImGui::SetWindowFontScale(1.7f); // Further increased from 1.5f
            ImGui::Text("%s", modelInfos[currentPopupModel].name.c_str());
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
            
            ImGui::Separator();
            
            // Monospace font kullanarak tek karakterli genişlikte fontla düzgün hizalama sağla
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 8)); // Increased spacing for better readability
            
            // Increase text size for description - FURTHER INCREASED
            ImGui::SetWindowFontScale(1.4f); // Further increased from 1.2f
            
            // Tüm metni satır satır ele alarak düzgün hizalama sağla
            std::istringstream iss(modelInfos[currentPopupModel].description);
            std::string line;
            while (std::getline(iss, line)) {
                ImGui::TextWrapped("%s", line.c_str());
            }
            
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleVar();
            ImGui::Separator();
            
            // Next button (centered) - INCREASED BUTTON SIZE MORE
            float buttonWidth = 180.0f; // Further increased from 150.0f
            float buttonHeight = 60.0f; // Further increased from 50.0f
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
            
            // Larger button font
            ImGui::SetWindowFontScale(1.3f);
            if (ImGui::Button("Next", ImVec2(buttonWidth, buttonHeight))) {
                if (waitingForUserInput) {
                    if (visitingModel) {
                        // Tur sırasında
                        int currentModelIndex = robot.getCurrentModelIndex();
                        
                        // Get robotGroundY for center position
                        float groundY = -5.0f;
                        float legHeight = 0.4f;
                        float bodyHeight = 0.4f;
                        float robotGroundY = groundY + legHeight + bodyHeight * 0.5f;
                        
                        // Merkezdeki pozisyon
                        glm::vec3 centerPos = roomCenter;
                        centerPos.y = robotGroundY;
                        
                        // Start moving back to center
                        robot.moveTo(centerPos);
                        robot.pointArmTo(centerPos);
                        
                        showModelPopup = false;
                        waitingForUserInput = false;
                        returningToCenter = true;
                        std::cout << "User clicked Next, returning to center from model " << (currentModelIndex + 1) << std::endl;
                    } else {
                        // Manuel tarama sırasında
                        showModelPopup = false;
                        waitingForUserInput = false;
                        manualScanningActive = false;
                        // modelScanned durumunu koruyoruz, sıfırlamıyoruz
                        std::cout << "User closed manual scanning info for model " << (currentPopupModel + 1) << std::endl;
                    }
                }
            }
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::End();
        }

        // Display scanning popup - FURTHER INCREASED SIZE
        if (showScanningPopup && scanningModelIndex >= 0 && scanningModelIndex < modelInfos.size()) {
            // Update scanning progress
            scanningProgress += scanningSpeed * 100.0f * deltaTime; // Saniyede yüzde 100 ilerleme
            scanningProgress = std::min(scanningProgress, 100.0f);
            
            // Calculate popup position (center of screen) - FURTHER INCREASED SIZE
            ImVec2 popupSize(450, 180); // Further increased from (400, 150)
            ImVec2 center = ImVec2(windowWidth * 0.5f, windowHeight * 0.3f);
            ImGui::SetNextWindowPos(ImVec2(center.x - popupSize.x * 0.5f, center.y - popupSize.y * 0.5f));
            ImGui::SetNextWindowSize(popupSize);
            
            // Create scanning popup
            ImGui::Begin("Scanning", NULL, 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            
            // Title with bold, blue text - FURTHER INCREASED FONT SIZE
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));  // Cyan color
            ImGui::SetWindowFontScale(2.0f); // Further increased from 1.8f
            ImGui::Text("Scanning Artifact...");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
            
            ImGui::Separator();
            
            // Progress bar - LARGER HEIGHT
            ImGui::SetWindowFontScale(1.2f);
            ImGui::ProgressBar(scanningProgress / 100.0f, ImVec2(-1, 30), // Increased height for better visibility 
                              (std::to_string((int)scanningProgress) + "%").c_str());
            
            // Larger font for message
            ImGui::SetWindowFontScale(1.4f);
            ImGui::TextWrapped("Collecting data from \"%s\"", modelInfos[scanningModelIndex].name.c_str());
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::End();
        }

        // Display intro popup when the application starts
        if (showIntroPopup) {
            // Calculate popup position (center of screen)
            ImVec2 popupSize(600, 350); // Width, height of the popup - slightly smaller than before
            ImVec2 center = ImVec2(windowWidth * 0.5f, windowHeight * 0.5f);
            ImGui::SetNextWindowPos(ImVec2(center.x - popupSize.x * 0.5f, center.y - popupSize.y * 0.5f));
            ImGui::SetNextWindowSize(popupSize);
            
            // Create intro popup with welcome message - enable scrolling
            ImGui::Begin("Welcome to Virtual Museum", NULL, 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoCollapse); // Removed NoScrollbar flag to enable scrolling
            
            // Title with larger gold font
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));  // Gold color
            ImGui::SetWindowFontScale(1.8f); // Slightly smaller than before (was 2.0f)
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Virtual Museum of Adana").x) * 0.5f);
            ImGui::Text("Virtual Museum of Adana");
            ImGui::PopStyleColor();
            
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Separator();
            
            // Content area with scroll
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -60), true); // Reserve space at bottom for button
            
            // Description with slightly larger font
            ImGui::SetWindowFontScale(1.3f); // Slightly smaller than before (was 1.4f)
            ImGui::Spacing();
            ImGui::TextWrapped("Welcome to the Virtual Museum application showcasing 3D models of historical artifacts from the Adana Museum.");
            ImGui::Spacing();
            ImGui::TextWrapped("This interactive environment allows you to explore five different statues and artifacts with detailed information in both Turkish and English.");
            ImGui::Spacing();
            ImGui::TextWrapped("You can control a robot to move around the museum using the arrow keys and scan each artifact to learn about its history and significance.");
            ImGui::Spacing();
            ImGui::TextWrapped("You can also start an automated guided tour where the robot will visit each artifact in sequence.");
            
            ImGui::Spacing();
            ImGui::Spacing();
            
            // Instructions section
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.9f, 1.0f, 1.0f));  // Light blue color
            ImGui::TextWrapped("Instructions:");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("• Use ARROW KEYS to move the robot around the museum");
            ImGui::TextWrapped("• Stand near an artifact for a few seconds to scan and view its information");
            ImGui::TextWrapped("• Click the \"Start Tour\" button for an automated guided experience");
            ImGui::TextWrapped("• Press ESC to exit the application");
            
            // Add some extra space at the bottom for demonstration of scrolling
            for (int i = 0; i < 3; i++) {
                ImGui::Spacing();
            }
            
            ImGui::TextWrapped("Scroll down to see more information about the project...");
            
            for (int i = 0; i < 3; i++) {
                ImGui::Spacing();
            }
            
            ImGui::TextWrapped("This virtual museum project showcases the integration of 3D modeling, interactive navigation, and educational content delivery in a single cohesive application.");
            
            ImGui::SetWindowFontScale(1.0f);
            ImGui::EndChild(); // End the scrolling region
            
            // Separator before button
            ImGui::Separator();
            
            // Close button at the center bottom
            float buttonWidth = 150.0f; // Slightly smaller button
            float buttonHeight = 40.0f; // Slightly smaller button
            
            // Center the button horizontally
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
            // The button is now positioned at the bottom by the layout
            
            ImGui::SetWindowFontScale(1.2f); // Slightly smaller font for button (was 1.3f)
            if (ImGui::Button("Close", ImVec2(buttonWidth, buttonHeight))) {
                showIntroPopup = false; // Hide the popup when Close is clicked
            }
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}