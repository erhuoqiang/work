#ifndef ANNOUNCE_H
#define ANNOUNCE_H

#include "listener.h"

namespace cvte {
namespace ui {

/**
 * @enum AnnounceMessage
 * @brief 广播消息类型
 */
typedef std::string AnnounceMessage;
//enum class AnnounceMessage {
//    kRefreshUI,     ///< 刷新UI消息
//};

typedef std::function<void(const AnnounceMessage&)> AnnounceCallback;

/**
 * @interface IAnnounceListener
 * @brief 广播监听接口
 */
class IAnnounceListener : public Listener<AnnounceMessage>::IListener {
public:
    virtual ~IAnnounceListener() {}
    /**
     * @brief 广播监听回调，监听者需重写该函数以实现对该消息的响应
     * @param message 广播消息类型
     * @param data 广播携带数据，函数内不需要释放该指针
     */
    virtual void OnAnnounce(const AnnounceMessage& message, void* data = nullptr) = 0;

    virtual void OnAction(const AnnounceMessage& message,
                          void* data /*= nullptr*/);
};

/**
 * @class LambdaAnnounceListener
 * @brief 广播监听接口的Lambda实现，允许使用Lambda表达式进行监听注册
 */
class LambdaAnnounceListener : public IAnnounceListener{
public:
    LambdaAnnounceListener();
    LambdaAnnounceListener(const AnnounceCallback& callback);

    virtual void OnAnnounce(const AnnounceMessage& message, void* data = nullptr);
private:
    AnnounceCallback callback_;
};

/**
 * @class Announce
 * @brief 广播监听行为的包装类\n
 * @brief 管理一组监听回调与对应消息的生存期
 */
class Announce {
public:
    using AnnounceMessages = std::set<AnnounceMessage>;
    /**
     * @brief 构造函数，调用 AnnounceManager 完成一组监听消息及监听回调的注册
     * @param messages 需要监听的一组消息集合
     * @param listener 监听回调
     */
    Announce(AnnounceMessages messages,
             std::shared_ptr<IAnnounceListener> listener);
    Announce(AnnounceMessages messages, const AnnounceCallback& callback);
    /**
     * @brief 析构函数，调用 AnnounceManager 完成监听消息的清空及监听回调的解注册
     */
    ~Announce();
    /**
     * @brief 调用 AnnounceManager 发布广播
     * @param message 广播的消息集合
     * @param data 广播携带数据
     */
    static void PublishMessage(const AnnounceMessage& message, void* data = nullptr);
private:
    AnnounceMessages messages_;
    std::shared_ptr<IAnnounceListener> announce_listener_;
};

/**
 * @class AnnounceManager
 * @brief 广播管理类
 */
class AnnounceManager {
public:
    static AnnounceManager* GetInstance();

    /**
     * @brief 完成一组监听消息及监听回调的注册
     * @param messages 需要监听的一组消息集合
     * @param listener 监听回调
     */
    void RegisterListener(const AnnounceMessage& message ,
                          std::shared_ptr<IAnnounceListener> handler);
    /**
     * @brief 完成监听消息的清空及监听回调的解注册
     * @param messages 需要解除监听的一组消息集合
     * @param listener 需要解除的监听回调
     */
    void UnRegisterListener(const AnnounceMessage& message ,
                            std::shared_ptr<IAnnounceListener> handler);
    /**
     * @brief 发布广播
     * @param message 广播的消息集合
     * @param data 广播携带数据
     */
    void PublishMessage(const AnnounceMessage& message, void* data = nullptr);
private:
    AnnounceManager();
    ~AnnounceManager();
    AnnounceManager(const AnnounceManager&) = delete;
    AnnounceManager& operator = (const AnnounceManager&) = delete;

private:
    Listener<AnnounceMessage> listener_;
};

} // ui
} // cvte

#endif // ANNOUNCE_H
