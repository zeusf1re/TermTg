#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

using namespace td::td_api;

class AuthHandler {
public:
  AuthHandler() : client_(std::make_unique<td::Client>()) {}

  void run() {
    while (true) {
      auto response = client_->receive(10.0);
      if (response.id == 0 || !response.object) {
        continue;
      }

      if (response.object->get_id() ==
          td::td_api::updateAuthorizationState::ID) {
        handle_auth_state(std::move(response.object));
      }
    }
  }

private:
  std::unique_ptr<td::Client> client_;

  void handle_auth_state(td::td_api::object_ptr<td::td_api::Object> update) {
    auto auth_state =
        td::move_tl_object_as<td::td_api::updateAuthorizationState>(update);

    switch (auth_state->authorization_state_->get_id()) {
    case td::td_api::authorizationStateWaitTdlibParameters::ID: {
      auto params = td::td_api::make_object<td::td_api::setTdlibParameters>();
      params = td::td_api::make_object<td::td_api::setTdlibParameters>();
      params->database_directory_ = "tdlib_data";
      params->use_message_database_ = true;
      params->use_secret_chats_ = true;
      params->api_id_ = 24903292;
      params->api_hash_ = "8404eb7d13afaabf05e324200c485a95";
      params->system_language_code_ = "en";
      params->device_model_ = "Terminal";
      params->application_version_ = "1.0";
      client_->send({1, std::move(params)});
      break;
    }

    case td::td_api::authorizationStateWaitPhoneNumber::ID: {
      std::cout
          << "Введите номер телефона (в международном формате, +79123456789): ";
      std::string phone_number;
      std::getline(std::cin, phone_number);

      client_->send(
          {1, td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(
                  phone_number, nullptr)});
      break;
    }

    case td::td_api::authorizationStateWaitCode::ID: {
      std::cout << "Введите код из SMS/Telegram: ";
      std::string code;
      std::getline(std::cin, code);

      client_->send(
          {1,
           td::td_api::make_object<td::td_api::checkAuthenticationCode>(code)});
      break;
    }

    case td::td_api::authorizationStateWaitPassword::ID: {
      std::cout << "Введите пароль 2FA: ";
      std::string password;
      std::getline(std::cin, password);

      client_->send(
          {1, td::td_api::make_object<td::td_api::checkAuthenticationPassword>(
                  password)});
      break;
    }

    case td::td_api::authorizationStateReady::ID: {
      std::cout << "Авторизация успешна!\n";
      break;
    }
    case td::td_api::updateNewMessage::ID: {
      auto msg = td::move_tl_object_as<td::td_api::updateNewMessage>(update);
      std::cout << "Получено новое сообщение!\n";
      break;
    } // После authorizationStateReady:

    default: {
      std::cout << "Неизвестное состояние авторизации\n";
    }
    }
  }
};
int main() {
  AuthHandler handler;
  handler.run();

  return 0;
}
