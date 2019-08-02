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

        std::unique_lock< std::mutex > finishedLock(vk::engine->finishedMutex);
        if (vk::engine->finished) break;
        finishedLock.unlock();

        std::unique_lock< std::mutex > lock(vk::engine->modelLoadingQueueMutex);
        vk::engine->modelLoadingQueueCondVar.wait(lock);

        if (!vk::engine->modelLoadingQueue.empty()) {
        
            auto front = vk::engine->modelLoadingQueue.front();
            vk::engine->modelLoadingQueue.pop();
            lock.unlock();

            Model* model = new Model(front.path, front.pipeline, front.lib);
            models.push_back(model);
        
        }

    }

}

std::vector< Model* > AssetLoader::get() {

    return models;

}