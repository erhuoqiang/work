#ifndef LISTERER_H
#define LISTERER_H

#include <map>
#include <set>
#include <memory>
#include <iostream>

namespace cvte {
namespace ui {

/**
 * @class Listener
 * @brief 广播监听行为的包装类\n
 * @brief 管理一组监听回调与对应消息的生存期
 */
template<typename T>
class Listener {
public:
    /**
     * @interface IListener
     * @brief 广播监听接口
     */
    class IListener {
      public:
        virtual ~IListener() {}
        /**
         * @brief 广播监听回调，监听者需重写该函数以实现对该消息的响应
         * @param message 广播消息类型
         * @param data 广播携带数据，函数内不需要释放该指针
         */
        virtual void OnAction(const T& message, void* data = nullptr) = 0;
    };

public:
    /**
     * @brief 构造函数，调用 AnnounceManager 完成一组监听消息及监听回调的注册
     * @param messages 需要监听的一组消息集合
     * @param listener 监听回调
     */
    Listener()
        : message_handler_map_() {
    }
    /**
     * @brief 析构函数，调用 AnnounceManager 完成监听消息的清空及监听回调的解注册
     */
    ~Listener() {
        message_handler_map_.clear();
    }

    using MessageSet = std::set<T>;

    /**
     * @brief 完成一组监听消息及监听回调的注册
     * @param messages 需要监听的一组消息集合
     * @param listener 监听回调
     */
    void Register(const T message , std::shared_ptr<Listener<T>::IListener> listener) {
        //std::cout << "RegisterListener:" << message << std::endl;
        auto iter = message_handler_map_.find(message);
        if (iter != std::end(message_handler_map_)) {
             if (iter->second.find(listener)  == end(iter->second)) {
                iter->second.emplace(listener);
             }
            return ;
        }
        message_handler_map_[message].emplace(listener);
    }
    /**
     * @brief 完成监听消息的清空及监听回调的解注册
     * @param messages 需要解除监听的一组消息集合
     * @param listener 需要解除的监听回调
     */
    void UnRegister(const T message , std::shared_ptr<Listener<T>::IListener> listener) {
        //std::cout << "UnRegisterListener:" << message << std::endl;
        auto iter = message_handler_map_.find(message);
        if (iter != std::end(message_handler_map_)) {
            auto iter_set = iter->second.find(listener);
            if (iter_set != std::end(iter->second)) {
                iter->second.erase(iter_set);
            }
        }
    }
    /**
     * @brief 发布广播
     * @param message 广播的消息集合
     * @param data 广播携带数据
     */
    void Publish(const T message, void* data = nullptr) {
        auto iter = message_handler_map_.find(message);
        if (iter != std::end(message_handler_map_)) {
            for (const auto& listener : iter->second) {
                auto listener_sptr = listener.lock();
                if (listener_sptr) {
                    listener_sptr->OnAction(message, data);
                }
            }
        }
    }

private:
    /**
     * @note 声明拷贝构造函数为 deleted 函数
     */
    Listener(const Listener&) = delete;
    /**
     * @note 声明拷贝赋值操作符为 deleted 函数
     */
    Listener& operator = (const Listener&) = delete;

private:
    // std::shared_ptr<IListener> listener_;
    using HandlerSet = std::set<std::weak_ptr<Listener<T>::IListener>,
                                std::owner_less<std::weak_ptr<Listener<T>::IListener>> >;
    std::map<T, HandlerSet> message_handler_map_;
};

} // ui
} // cvte

#endif // LISTERER_H
