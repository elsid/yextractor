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

```
DEFINE_PARAMETER(имя_типа, тип_значения, "ключ")
DEFINE_PARAMETER_WITH_PARSER(имя_типа, тип_значения, "ключ", тип_парсера)
```

Пример:

```DEFINE_PARAMETER(SomeParameter, std::string, "some")```

## Тип парсера

Должен иметь интерфейс:

```
struct Parser {
    yamail::yextractor::Errors operator ()(std::string& dst, const std::string& src) const;
};
```

Пример:
```
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

Объект создается для каждого значения параметра конструктором по-умолчанию.

## Выражения для верификации

Позволяют выполнить верификацию комбинации параметров, извлеченной из коллекции.

### Типы выражений

#### Required

```template Required <class Parameter>```

Задает обязательный параметр. Верификация проходит, если значение параметра прошло верификацию.

Пример:

```using GetSomeRequiredParameter = Required<SomeParameter>;```

#### Optional

```template Optional <class Parameter>```

Задает необязательный параметр. Верификация проходит всегда.

Пример:
```
using GetSomeOptionalParameter = Optional<SomeParameter>;
```

#### Any

```template Any <class ... Values>```

Задает набор выражений. Верификация проходит, если хотя бы одно значение прошло верификацию.
Заполняются все прошедшие верификацию значения.

Пример:

```using GetAnyParameter = Any<SomeParameter, AnotherParameter>;```

#### First

```template First <class ... Values>```

Задает набор выражений. Верификация проходит, если хотя бы одно значение прошло верификацию.
Заполняется первое прошедшее верификацию значение.

Пример:

```using GetFirstParameter = First<SomeParameter, AnotherParameter>;```

#### Every

```template Every <class ... Values>```

Задает набор выражений. Верификация проходит, если все значения прошли верификацию.

Пример:

```using GetEveryParameter = Every<SomeParameter, AnotherParameter>;```

### Сочетания типов выражений

```Required``` и ```Optional``` можно использовать только для параметров.
```Any```, ```First``` и ```Every``` можно использовать в любых сочетаниях.

Примеры:
```
Any<Every<A, B>, Every<C, D>>
Every<Any<A, B>, Any<C, D>>
Every<A, B, Optional<C>>
```

## Извлечение значений

Для этого нужно использовать объект класса ```Extractor```.
Метод ```get``` возвращает ```std::tuple``` от всех параметров в выражении.
Если в выражении параметры дублируются, в результате будет так же.

## Ошибки верификации

Метод ```Extractor::errors``` возвращает объекта класса ошибок ```Errors```,
в котором содержится стек сообщений об ошибках ветви выражения, приведшей к
провалу верфикации всего выражения. Сообщения упорядочены сверху вниз.
