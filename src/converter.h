
/**
 * PyOtherSide: Asynchronous Python 3 Bindings for Qt 5
 * Copyright (c) 2011, 2013, Thomas Perl <m@thp.io>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 **/

#ifndef PYOTHERSIDE_CONVERTER_H
#define PYOTHERSIDE_CONVERTER_H

template<class V>
class ListBuilder {
    public:
        ListBuilder() {}
        virtual ~ListBuilder() {}

        virtual void append(V) = 0;
        virtual V value() = 0;
};

template<class V>
class DictBuilder {
    public:
        DictBuilder() {}
        virtual ~DictBuilder() {}

        virtual void set(V, V) = 0;
        virtual V value() = 0;
};

template<class V>
class ListIterator {
    public:
        ListIterator() {}
        virtual ~ListIterator() {}

        virtual int count() = 0;
        virtual bool next(V*) = 0;
};

template<class V>
class DictIterator {
    public:
        DictIterator() {}
        virtual ~DictIterator() {}

        virtual bool next(V*, V*) = 0;
};

template<class V>
class Converter {
    public:
        Converter() {}
        virtual ~Converter() {}

        enum Type {
            NONE = 0,
            INTEGER,
            FLOATING,
            BOOLEAN,
            STRING,
            LIST,
            DICT,
        };

        virtual enum Type type(V&) = 0;
        virtual long long integer(V&) = 0;
        virtual double floating(V&) = 0;
        virtual bool boolean(V&) = 0;
        virtual const char *string(V&) = 0;
        virtual ListIterator<V> *list(V&) = 0;
        virtual DictIterator<V> *dict(V&) = 0;

        virtual V fromInteger(long long v) = 0;
        virtual V fromFloating(double v) = 0;
        virtual V fromBoolean(bool v) = 0;
        virtual V fromString(const char *v) = 0;
        virtual ListBuilder<V> *newList() = 0;
        virtual DictBuilder<V> *newDict() = 0;
        virtual V none() = 0;
};

template<class F, class T, class FC, class TC>
T
convert(F from)
{
    FC fconv;
    TC tconv;

    switch (fconv.type(from)) {
        case FC::NONE:
            return tconv.none();
        case FC::INTEGER:
            return tconv.fromInteger(fconv.integer(from));
        case FC::FLOATING:
            return tconv.fromFloating(fconv.floating(from));
        case FC::BOOLEAN:
            return tconv.fromBoolean(fconv.boolean(from));
        case FC::STRING:
            return tconv.fromString(fconv.string(from));
        case FC::LIST:
            {
                ListBuilder<T> *listBuilder = tconv.newList();

                F listValue;
                ListIterator<F> *listIterator = fconv.list(from);
                while (listIterator->next(&listValue)) {
                    listBuilder->append(convert<F, T, FC, TC>(listValue));
                }
                delete listIterator;

                T listResult = listBuilder->value();
                delete listBuilder;

                return listResult;
            }
        case FC::DICT:
            {
                DictBuilder<T> *dictBuilder = tconv.newDict();

                DictIterator<F> *dictIterator = fconv.dict(from);
                FC keyConvFrom;
                TC keyConvTo;
                F dictKey;
                F dictValue;
                while (dictIterator->next(&dictKey, &dictValue)) {
                    dictBuilder->set(keyConvTo.fromString(keyConvFrom.string(dictKey)), convert<F, T, FC, TC>(dictValue));
                }
                delete dictIterator;

                T dictResult = dictBuilder->value();
                delete dictBuilder;

                return dictResult;
            }
    }

    return tconv.none();
}

#endif /* PYOTHERSIDE_CONVERTER_H */