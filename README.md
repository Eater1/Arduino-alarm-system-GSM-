# Система сигнализации с оповещением/управлением через SMS-сообщения
---

##  Используемые оборудования

Список используемых оборудований при проектировании системы сигнализации:
1) Микроконтроллер Arduino Uno;
2) Плата GPRS Shield на основе SIM900R;
3) Датчик движения “MH-20”;
4) Оповещатель “Гром-12K”. 


## Макетная плата сигнальной системы

![prototyping-board-layout1](https://user-images.githubusercontent.com/11851207/27731080-fef797a2-5dce-11e7-8e0e-3f6d64d38894.jpg)

## Электрическая принципиальная схема сигнальной системы

![schematic-diagram1](https://user-images.githubusercontent.com/11851207/27731143-34d90554-5dcf-11e7-93a8-7dbca573ac4b.jpg)

## Внешний вид компьютерного интерфейса

Через компьютерный интерфейс пользователь может задать определенные настройки для сигнализации (время взвода, текст оповещения и номер телефона который нужно оповещать).
![1](https://user-images.githubusercontent.com/11851207/27733305-bf241ace-5dd8-11e7-8e0d-51bfcc6bc288.jpg)

## Описание работы схемы

Вся схема охранной сигнализации запитана от выносного блока питания постоянного напряжения 12 вольт. Постоянное напряжение 12 вольт подаем на Arduino Uno и датчик движения (“MH-20”), а модуль GSM Shield запитывается постоянным напряжением 5 вольт от модуля Arduino Uno, который имеет стабилизатор напряжения от 7 до 12 вольт.
Рассмотрим работу данной схемы. Кнопка SW1 служит для включения охранной сигнализации.  Кратковременное нажатие на кнопку SW1, при этом подается уровень логического нуля на вход D3 Arduino Uno, в остальное время на входе D3 присутствует уровень логической единицы за счет резистора R1,  приводит к запуску таймера на 10 секунд (об этом сигнализирует мигание желтого светодиода VD1). Данное время необходимо для того, чтобы успеть покинуть охраняемое помещение без срабатывания  сигнализации. При этом визуальный  контроль шлейфа осуществляем по красному светодиоду VD2, на время работы таймера, при попадании в зону срабатывания датчика движения, светодиод VD2 начинает мигать, но при этом нет посылки тревожного сообщения, а при покидании зоны срабатывания датчика движения, шлейф вновь становится на контроль автоматически, о чем свидетельствует постоянное свечение светодиода VD2. Управление светодиодами осуществляет блок Arduino Uno выходы D11, D12 путем выдачи логической единицы, резисторы R2 и R3 в цепях светодиодов являются токо ограничивающими. 
Резисторы R1 и R4 служат для подачи “гарантированной” логической единицы на входы Arduino Uno при размыкании цепей.
По истечении времени таймера, система переходит в режим охраны, о чём свидетельствует погасание желтого светодиода VD1. В роли шлейфа мы используем контакты реле датчика движения, которые в режиме охраны замкнуты, через эти контакты уровень логического нуля подается на вход D10 модуля Arduino Uno.  При попадании постороннего объекта в зону действия датчика контакты реле размыкаются, что приводит к подаче уровня “гарантированной” логической единице на вход VD10 за счет резистора R4. При этом Arduino Uno по выходам D6, D7 (COM порт) передает команду GSM модулю на передачу SMS сообщения на определенный сотовый номер. С выхода D4 идет сигнал на оповещатель для подачи светового сигнала.  Так как оповещатель «Гром-12К» работает от 12 вольт нам понадобиться реле, которое коммутирует напряжение  12 вольт. При нарушении шлейфа, с выхода D4 Arduino уровень логической единицы (5 вольт) подается на катушку реле, другой контакт которой соединен с нулем. Реле KV1 срабатывает и переключающий контакт подает +12 вольт на оповещатель, что приводит к включению звуковой сирены.

## Пример работы GSM сигнализации

Пример работы можно увидеть по ссылке: https://youtu.be/kimXZzf4bYE
