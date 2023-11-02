# 36.7. Итоговый проект Серверная часть...
  
    Реализация GUI для серверной части приложения (от 0 до 10 баллов):
    Наличие окна просмотра всех сообщений и списка пользователей (5 балла)
    Возможность отключения и бана пользователей (5 балла)

## Реализованно:
      QTCPServrt с базой данных mysql
      При первом старте сервера открывается окно с предложением ввести данные для доступа к mysql
      В последующие разы сервер стартует автоматически при запуске.
      Реализованна возможность запуска истановки и отклчения клиентов
      Реализованн просмотр базы данных с возможностью удаления пользователей / сообщений
      Возможность забанить пользователя сделать/админом/удалить. удаленный пользователь больше не сможет авторизоваться

  ### Установка:
      скачать проект,
      создать каталог realise рядом с проектом,
      перейти в этот каталог,
      выполнить:  cmake 'путь до каталога с проектом'
      выполлнить: make

      В случае если при старте приложения будет предупреждение что не загрузились файлы перевода,
      то поместите переводы из папки 'translations' в папку 'Server_translations' по пути из сообщения!
      

### Требования:  
    На PC долджна быть установленна и настроена mysql,
    Вы должны создать базу данных и пользователя для нее.
    Программа запросит имя БД
    логин
    пароль 
    имя хоста
    После успешного подключения программа создаст дефолтные таблицы БД


### Компиляция:



https://github.com/ivanKispaj/Server_QT/assets/91827767/383f2617-bf28-47b3-998e-ea46c2366b46

### Демо:




https://github.com/ivanKispaj/Server_QT/assets/91827767/43b6375e-3e84-4a9d-8914-5adb63b805d6




