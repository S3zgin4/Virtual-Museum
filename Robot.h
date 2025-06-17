#ifndef ROBOT_H
#define ROBOT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "GameObject.h"
#include "RobotBody.h"
#include "RobotArm.h"
#include "Mesh.h"

class Robot {
private:
    std::unique_ptr<RobotBody> body;
    std::unique_ptr<RobotArm> arm;
    glm::vec3 position;
    glm::vec3 targetPosition;  // Hedef konum
    bool isMoving;             // Hareket durumu
    float moveSpeed;           // Hareket h�z�
    float legAnimationTime;    // Bacak animasyonu i�in zaman
    float legAnimationSpeed;   // Bacak animasyon h�z�
    float currentRotation;     // �u anki rotasyon a��s� (derece)
    glm::vec3 scale;           // �l�eklendirme

    // Otomatik tur i�in de�i�kenler
    bool isTouringModels;      // Otomatik tur durumu
    int currentModelIndex;     // �u anki model indeksi
    float waitTimer;           // Her model aras�nda bekleme s�resi
    bool waitingBetweenModels; // Modeller aras� bekleme durumu

    struct Leg {
        std::unique_ptr<Mesh> mesh;
        glm::vec3 pos;
        glm::vec3 scale;
        float offsetY;         // Animasyon i�in Y ekseninde offset
    };
    std::vector<Leg> legs;

public:
    Robot() {
        float legHeight = 0.4f;   // Ayak y�ksekli�i (kare prizma)
        float legWidth = 0.25f;   // Ayak geni�li�i (daha ince)
        float legDepth = 0.25f;   // Ayak derinli�i (daha ince)
        float bodyHeight = 0.4f;  // G�vde y�ksekli�i
        float bodyWidth = 0.5f;   // G�vde geni�li�i (RobotBody'den)
        float bodyDepth = 0.5f;   // G�vde derinli�i (RobotBody'den)

        // Robot'u zemine oturtmak i�in -5.0f (zemin seviyesi) + legHeight (tam ayak y�ksekli�i) + bodyHeight/2 (g�vde merkezi)
        position = glm::vec3(0.0f, -5.0f + legHeight + bodyHeight * 0.5f, 0.0f);
        targetPosition = position;
        isMoving = false;
        moveSpeed = 4.0f;  // Daha h�zl� hareket (�nceki: 2.5f)
        legAnimationTime = 0.0f;
        legAnimationSpeed = 6.0f; // Daha h�zl� bacak animasyonu (�nceki: 5.0f)
        currentRotation = 0.0f;   // Ba�lang��ta rotasyon 0
        scale = glm::vec3(1.0f);  // Varsay�lan �l�eklendirme
        
        // Tur de�i�kenlerini ba�lat
        isTouringModels = false;
        currentModelIndex = 0;
        waitTimer = 0.0f;
        waitingBetweenModels = false;

        body = std::make_unique<RobotBody>();

        // 4 k��e i�in kare prizma ayaklar - daha fazla d��ar� yerle�tirme (0.6f -> 0.8f)
        glm::vec3 legOffsets[] = {
            glm::vec3(-bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f,  bodyDepth * 0.8f),
            glm::vec3( bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f,  bodyDepth * 0.8f),
            glm::vec3(-bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f, -bodyDepth * 0.8f),
            glm::vec3( bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f, -bodyDepth * 0.8f)
        };
        
        // Her bacak i�in farkl� renkler
        glm::vec4 legColors[] = {
            glm::vec4(0.5f, 0.2f, 0.2f, 1.0f),  // K�rm�z�ms� kahverengi
            glm::vec4(0.2f, 0.5f, 0.2f, 1.0f),  // Ye�ilimsi
            glm::vec4(0.2f, 0.2f, 0.5f, 1.0f),  // Mavimsi
            glm::vec4(0.5f, 0.5f, 0.2f, 1.0f)   // Sar�ms�
        };
        
        for (int i = 0; i < 4; ++i) {
            Leg leg;
            leg.mesh = std::make_unique<Mesh>(legColors[i]);
            leg.scale = glm::vec3(legWidth, legHeight, legDepth); // Kare prizma
            leg.pos = position + legOffsets[i];
            leg.offsetY = 0.0f;
            legs.push_back(std::move(leg));
        }

        arm = std::make_unique<RobotArm>();
        arm->setPosition(glm::vec3(0.0f, position.y + bodyHeight * 0.5f, 0.0f));

        setPosition(position);
    }

    void setPosition(const glm::vec3& newPosition) {
        float legHeight = 0.4f;   // Ayak y�ksekli�i
        float legWidth = 0.25f;   // Ayak geni�li�i
        float legDepth = 0.25f;   // Ayak derinli�i
        float bodyHeight = 0.4f;  // G�vde y�ksekli�i
        float bodyWidth = 0.5f;   // G�vde geni�li�i
        float bodyDepth = 0.5f;   // G�vde derinli�i

        position = newPosition;
        
        // G�vde ve kolu 270 derece sa�a d�nd�r (Mevcut rotasyona ek olarak)
        float bodyRotation = currentRotation + 270.0f; // 180 derece yerine 270 derece
        body->setPosition(position);
        body->setRotation(glm::vec3(0.0f, bodyRotation, 0.0f)); // G�vdeyi 270 derece sa�a d�nd�r

        // Bacak pozisyonlar� ayn� kal�r - ayaklar normal y�nlerinde olacak
        float angle = glm::radians(currentRotation);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);

        // Orijinal bacak offsetleri (rotasyon uygulanmadan �nce)
        glm::vec3 originalLegOffsets[] = {
            glm::vec3(-bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f,  bodyDepth * 0.8f),
            glm::vec3( bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f,  bodyDepth * 0.8f),
            glm::vec3(-bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f, -bodyDepth * 0.8f),
            glm::vec3( bodyWidth * 0.8f, -bodyHeight * 0.5f - legHeight * 0.5f, -bodyDepth * 0.8f)
        };

        // Rotasyonu uygulayarak bacak pozisyonlar�n� hesapla
        for (size_t i = 0; i < legs.size(); i++) {
            // Y ekseni etraf�nda d�nd�rme
            glm::vec3 rotatedOffset = originalLegOffsets[i];
            float originalX = originalLegOffsets[i].x;
            float originalZ = originalLegOffsets[i].z;
            
            rotatedOffset.x = originalX * cosAngle - originalZ * sinAngle;
            rotatedOffset.z = originalX * sinAngle + originalZ * cosAngle;
            
            legs[i].pos = position + rotatedOffset;
            
            // E�er animasyon varsa ofseti uygula
            legs[i].pos.y += legs[i].offsetY;
        }

        // Kol pozisyonunu g�ncelle (g�vdenin �st�nde kalmal�)
        glm::vec3 armPos = position;
        armPos.y += bodyHeight * 0.5f;
        arm->setPosition(armPos);
        arm->setRotation(glm::vec3(0.0f, bodyRotation, 0.0f)); // Kolu g�vde ile ayn� rotasyona getir
    }

    void setScale(const glm::vec3& newScale) {
        scale = newScale;
        if (body) body->setScale(newScale);
        if (arm) arm->setScale(newScale);
    }

    void update(float deltaTime) {
        // Debug bilgisi i�in
        if (isTouringModels) {
            std::cout << "Tour status - Moving: " << (isMoving ? "Yes" : "No")
                      << ", Waiting: " << (waitingBetweenModels ? "Yes" : "No") 
                      << ", Current Model: " << (currentModelIndex + 1)
                      << ", Wait Timer: " << waitTimer << std::endl;
        }
        
        // Y�r�me animasyonu i�in bacaklar� hareket ettir
        if (isMoving) {
            // Zaman ilerlemesi
            legAnimationTime += deltaTime * legAnimationSpeed;
            
            // Hedef konuma do�ru hareket
            glm::vec3 direction = targetPosition - position;
            
            // G�vde rotasyonu (hedef y�n�nde)
            if (glm::length(direction) > 0.01f) {
                float angle = atan2(direction.x, direction.z);
                currentRotation = glm::degrees(angle); // A��y� derece olarak sakla
                
                // G�vde ve kolu 270 derece sa�a d�nd�rme (180 derece yerine)
                float bodyRotation = currentRotation + 270.0f;
                body->setRotation(glm::vec3(0.0f, bodyRotation, 0.0f));
                
                // Hareket y�n�nde ileriye bakmas� i�in bir hedef nokta olu�tur
                glm::vec3 forwardPoint = position + direction * 10.0f;
                // Robotun kolunu hareket y�n�nde bakacak �ekilde ayarla
                pointArmTo(forwardPoint);
            }
            
            // E�er hedefe �ok yak�nsa hareketi durdur
            if (glm::length(direction) < 0.1f) {
                isMoving = false;
                // Ayaklar� normal pozisyona getir
                for (auto& leg : legs) {
                    leg.offsetY = 0.0f;
                }
                setPosition(position); // Son pozisyonda d�zg�n hizalama i�in
                
                // E�er tur modundaysa, bir sonraki modele ge�mek i�in bekleme s�resini ba�lat
                if (isTouringModels) {
                    waitTimer = 0.5f; // 0.5 saniye bekle
                    waitingBetweenModels = true;
                    
                    std::cout << "Robot arrived at model " << (currentModelIndex + 1) 
                              << ", waiting for " << waitTimer << " seconds" << std::endl;
                }
            } else {
                // Y�n vekt�r�n� normalize et
                if (glm::length(direction) > 0.0f) {
                    direction = glm::normalize(direction);
                }
                
                // Hedefe do�ru hareket et
                float step = moveSpeed * deltaTime;
                float distance = glm::length(targetPosition - position);
                
                // E�er ad�m mesafeden b�y�kse, mesafe kadar hareket et
                if (step > distance) {
                    step = distance;
                }
                
                position += direction * step;
                
                // Bacak animasyonu (ad�mlar i�in yukar� kald�rma/indirme)
                for (size_t i = 0; i < legs.size(); i++) {
                    // 0 ve 3 ayn� fazda, 1 ve 2 ters fazda hareket etsin
                    float phase = (i % 2 == 0) ? sin(legAnimationTime) : sin(legAnimationTime + glm::pi<float>());
                    legs[i].offsetY = 0.07f * phase; // Daha belirgin yukar�/a�a�� hareket (�nceki: 0.05f)
                }
                
                // Pozisyonu g�ncelle
                setPosition(position);
            }
        }
        // Tur modundaysa ve bekleme durumundaysa
        else if (isTouringModels && waitingBetweenModels) {
            // Bekleme s�resini azalt
            waitTimer -= deltaTime;
            
            // E�er bekleme s�resi dolduysa modele ge�
            if (waitTimer <= 0.0f) {
                waitingBetweenModels = false;
            }
        }

        body->update(deltaTime);
        arm->update(deltaTime);
    }

    void pointArmTo(const glm::vec3& target) {
        // Kolun taban pozisyonu (robotun �st�)
        float bodyHeight = 0.4f;  // G�vde y�ksekli�i
        
        glm::vec3 armPos = position;
        armPos.y += bodyHeight * 0.5f;
        
        glm::vec3 toTarget = target - armPos;
        float yaw = atan2(toTarget.x, toTarget.z);
        float pitch = -atan2(toTarget.y, glm::length(glm::vec2(toTarget.x, toTarget.z)));

        // Kolun rotasyonunu ayarla (Y ekseni: yaw, X ekseni: pitch)
        arm->setRotation(glm::vec3(glm::degrees(pitch), glm::degrees(yaw), 0.0f));
    }

    void moveTo(const glm::vec3& targetPos) {
        targetPosition = targetPos;
        isMoving = true;
    }

    bool isWaitingBetweenModels() const {
        return waitingBetweenModels;
    }

    int getCurrentModelIndex() const {
        return currentModelIndex;
    }

    bool isCurrentlyMoving() const {
        return isMoving;
    }

    bool isCurrentlyTouring() const {
        return isTouringModels;
    }

    // Pause the current tour
    void pauseTour() {
        if (isTouringModels && !isMoving) {
            std::cout << "Tour paused at model " << (currentModelIndex + 1) << std::endl;
            isTouringModels = false;
        }
    }

    // Resume a paused tour
    void resumeTour() {
        if (!isTouringModels) {
            std::cout << "Tour resumed from model " << (currentModelIndex + 1) << std::endl;
            isTouringModels = true;
            waitingBetweenModels = false;
        }
    }

    // Jump to a specific model in the tour
    void jumpToModel(int modelIndex, int totalModels) {
        if (modelIndex >= 0 && modelIndex < totalModels) {
            currentModelIndex = modelIndex;
            waitingBetweenModels = false;
            std::cout << "Jumping to model " << (currentModelIndex + 1) << std::endl;
        }
    }

    // Set the wait time between models
    void setWaitTime(float seconds) {
        float minWait = 0.1f;
        waitTimer = (seconds > minWait) ? seconds : minWait;
        std::cout << "Wait time between models set to " << waitTimer << " seconds" << std::endl;
    }

    // Reset the tour to start from the beginning
    void resetTour() {
        currentModelIndex = 0;
        waitingBetweenModels = false;
        std::cout << "Tour reset to first model" << std::endl;
    }

    // Get information about the current tour status
    std::string getTourStatus() const {
        std::string status = "Tour Status: ";
        if (!isTouringModels)
            status += "Not touring";
        else if (isMoving)
            status += "Moving to model " + std::to_string(currentModelIndex + 1);
        else if (waitingBetweenModels)
            status += "Waiting at model " + std::to_string(currentModelIndex + 1) + 
                    " (" + std::to_string(waitTimer) + "s remaining)";
        else
            status += "Ready to move to next model";
        return status;
    }

    // Adjust robot's movement speed
    void setMoveSpeed(float speed) {
        moveSpeed = (speed > 0.0f) ? speed : 1.0f;
        std::cout << "Robot speed set to " << moveSpeed << std::endl;
    }

    // Check if we've completed a full tour
    bool hasCompletedTour(int totalModels) const {
        return isTouringModels && currentModelIndex == 0 && waitingBetweenModels;
    }

    void startModelTour() {
        isTouringModels = true;
        currentModelIndex = 0;
        waitingBetweenModels = false;
        isMoving = false;
        waitTimer = 0.0f;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    // Mevcut rotasyon a��s�n� al
    float getCurrentRotation() const {
        return currentRotation;
    }

    // Belirli bir miktar a�� ile d�n
    void rotateBy(float angleInDegrees) {
        currentRotation += angleInDegrees;
        
        // A��y� 0-360 aral���nda tut
        while (currentRotation >= 360.0f)
            currentRotation -= 360.0f;
        while (currentRotation < 0.0f)
            currentRotation += 360.0f;
        
        // D�nd�rme i�leminden sonra pozisyonu g�ncelleyerek 
        // robotun g�vdesinin ve kolunun y�nelimini g�ncelle
        setPosition(position);
    }

    void render(Shader& shader) {
        // �nce ayaklar� �iz
        for (const auto& leg : legs) {
            shader.Activate();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, leg.pos);
            model = glm::scale(model, leg.scale);
            shader.setMat4("model", model);
            leg.mesh->draw();
        }
        
        // Sonra g�vde ve kolu �iz
        body->render(shader);
        arm->render(shader);
    }
};

#endif // ROBOT_H