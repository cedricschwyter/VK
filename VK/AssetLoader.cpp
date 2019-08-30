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

        std::unique_lock< std::mutex > lock(VKCore::modelLoadingQueueMutex);
        VKCore::modelLoadingQueueCondVar.wait(lock, []() {
                
            return VKCore::notified || VKCore::finished;
                
            });

        if (!VKCore::modelLoadingQueue.empty()) {

            auto front = VKCore::modelLoadingQueue.front();
            VKCore::modelLoadingQueue.pop();
            VKCore::notified = false;
            lock.unlock();

            Model* model = new Model(front.path, front.pipeline, front.lib, front.modelMatrixFunc);
            models.push_back(model);

        }
        else if (VKCore::finished) { 

            break; 
        
        }
        
    }

}

std::vector< Model* > AssetLoader::get() {

    return this->models;

}