yextractor
==========

# Описание

Библиотека для извлечения типизированного набора параметров из key-value
коллекций с верификацией выражений и парсингом значений параметров.

## Источник данных

Можно использовать любую коллекцию, поддерживающую метод ```std::map::find```.
Ключем и значениями должен быть тип ```std::string```,
например ```std::map<std::string, std::string>```.

## Именованные параметры

Нужно определить типы параметров, используя макрос ```DEFINE_PARAMETER``` или
```DEFINE_PARAMETER_WITH_PARSER``` для использования специализированного парсера.

```c++
DEFINE_PARAMETER(type_name, value_type, "key")
DEFINE_PARAMETER_WITH_PARSER(type_name, value_type, "key", parser_type)
```

Подробнее см. [код](include/yamail/yextractor/parameter.hpp).

Пример:

```c++
DEFINE_PARAMETER(SomeParameter, std::string, "some")
```

## Тип парсера

Должен иметь интерфейс:

```c++
struct Parser {
    yamail::yextractor::Errors operator ()(T& dst, const std::string& src) const;
};
```

```T``` - тип значения параметра

Пример:
```c++
struct ParamParser {
    Errors operator ()(std::string& dst, const std::string& src) const {
        if (!boost::starts_with(src, "prefix")) {
            return Errors("value does't not starts with 'prefix'");
        }
        dst = src;
        return Errors();
    }
};

DEFINE_PARAMETER_WITH_PARSER(AnotherParameter, std::string, "another", ParamParser)
```

Объект парсера создается для каждого значения параметра конструктором по-умолчанию.

## Выражения для верификации

Позволяют выполнить верификацию комбинации параметров, извлеченной из коллекции.

### Типы выражений

#### Required

```c++
template Required <class Parameter>
```

Задает обязательный параметр. Верификация проходит, если значение параметра прошло верификацию.

Пример:

```c++
using GetSomeRequiredParameter = Required<SomeParameter>;
```

#### Optional

```c++
template Optional <class Parameter>
```

Задает необязательный параметр. Верификация проходит всегда.

Пример:
```c++
using GetSomeOptionalParameter = Optional<SomeParameter>;
```

#### Any

```c++
template Any <class ... Values>
```

Задает набор выражений. Верификация проходит, если хотя бы одно значение прошло верификацию.
Заполняются все прошедшие верификацию значения.

Пример:

```c++
using GetAnyParameter = Any<SomeParameter, AnotherParameter>;
```

#### First

```c++
template First <class ... Values>
```

Задает набор выражений. Верификация проходит, если хотя бы одно значение прошло верификацию.
Заполняется первое прошедшее верификацию значение.

Пример:

```c++
using GetFirstParameter = First<SomeParameter, AnotherParameter>;
```

#### Every

```c++
template Every <class ... Values>
```

Задает набор выражений. Верификация проходит, если все значения прошли верификацию.

Пример:

```c++
using GetEveryParameter = Every<SomeParameter, AnotherParameter>;
```

### Сочетания типов выражений

```Required``` и ```Optional``` можно использовать только для параметров.
```Any```, ```First``` и ```Every``` можно использовать в любых сочетаниях.

Примеры:
```c++
using Example1 = Any<Every<A, B>, Every<C, D>>;
using Example2 = Every<Any<A, B>, Any<C, D>>;
using Example3 = Every<A, B, Optional<C>>;
```

## Извлечение значений

Для этого нужно использовать объект класса ```Extractor```.
Метод ```get``` возвращает ```std::tuple``` от всех параметров в выражении.
Если в выражении параметры дублируются, в результате будет так же.
Значения обернуты в ```detail::Value``` (аналог ```boost::optional```).
Тип результата можно получить с помощью ```Expression<T>::Type```.

Объекты классов именованных параметров можно создавать на основе результата с
помощью конструкторов:
```c++
template <class ... Values>
Parameter(const std::tuple<Values ...>& values);
```

Копирует значение из результата (см. [тест](src/tests/parameter.cpp#L25-L37)).

```c++
template <class ... Values>
Parameter(std::tuple<Values ...>& values);
```

Перемещает значение из результата. Повторное применение создаст
неинициализированный параметр (см. [тест](src/tests/parameter.cpp#L39-L50)).

Проверить, инициализирован ли параметр, можно с помощью метода:
```c++
bool Parameter::initialized() const;
```

Значение параметра можно получить по константной ссылке (см. [тест](src/tests/parameter.cpp#L65-L69)):
```c++
const Parameter::Type& Parameter::get() const;
```

Или по universal reference (см. [тест](src/tests/parameter.cpp#L71-L77)):
```c++
Parameter::Type&& Parameter::take();
```

## Ошибки верификации

Метод ```Extractor::errors``` возвращает объекта класса ошибок ```Errors```,
в котором содержится стек сообщений об ошибках ветви выражения, приведшей к
провалу верфикации всего выражения. Сообщения упорядочены сверху вниз.

# Примеры

См. [здесь](src/examples/main.cpp)
