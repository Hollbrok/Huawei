# TODO

- [ ] Добавить функцию для удобного вывода числа в bitset (с нумерацией битов выше) + учтение режима дебага
- [ ] Тест прогу вынеси в отдельный файл (c beq + add)

- [ ] Обрабатывающую функцию устанавливать в том же месте, где и тип, чтобы лишний раз не искать нужную, ведь на месте, где определили тип уже все известно.

- [ ] Добавить несколько уровней DEBUG_REGIME по приоритету




## IMPORTANT

- [ ]  Опция симулятора для печати исполняемых инструкций со значениями регистров используемых/модифицируемых инструкцией. Поощряются обобщённые решения (сложный вариант).

(Можно сделать executor не просто указателем на функцию, а функтором, в качестве аргументо конструктора будет принимать нужные параметра, тем самым сократить кол-во кода и копи-пасты)

- [ ] Корректная поддержка обработки регистра x0 (на лекции не рассматривалось). Оригинальные решения поощряются (сложный вариант).
