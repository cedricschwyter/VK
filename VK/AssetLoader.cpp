/**
    Implementation of the AssetLoader functor class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         AssetLoader.cpp
    @brief        Implementation of the AssetLoader functor class
*/
#include "AssetLoader.hpp"
#include "VK.hpp"


AssetLoader::AssetLoader() {



}

void AssetLoader::operator()() {

    while (true) {

        std::unique_lock< std::mutex > lock(vk::core::modelLoadingQueueMutex);
        vk::core::modelLoadingQueueCondVar.wait(lock, []() {
                
            return vk::core::notified || vk::core::finished;
                
            });

        if (!vk::core::modelLoadingQueue.empty()) {

            auto front = vk::core::modelLoadingQueue.front();
            vk::core::modelLoadingQueue.pop();
            vk::core::notified = false;
            lock.unlock();

            Model* model = new Model(front.path, front.pipeline, front.lib, front.modelMatrixFunc);
            models.push_back(model);

        }
        else if (vk::core::finished) { 

            break; 
        
        }
        
    }

}

std::vector< Model* > AssetLoader::get() {

    return this->models;

}