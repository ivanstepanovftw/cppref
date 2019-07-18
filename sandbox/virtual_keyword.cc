#include <iostream>
#include <vector>
#include <memory>

using namespace std; // Плохая практика: использование `using` - в публичных файлах, хорошая: в приватных.



class Base {
public:
    Base() { cout << "Base::Base()\n"; }

    ~Base() { cout << "Base::~Base()\n"; }

    void bar() { cout << "Base::bar()\n"; }

    virtual void foo() { cout << "Base::foo()\n"; }
};

class Derived : public Base { // Плохая практика: наследуемся от класса, который не имеет виртуальный деструктор. Пример UB ниже.
public:
    Derived() { cout << "Derived::Derived()\n"; v.reserve(10); p = new int(10); }

    ~Derived() { cout << "Derived::~Derived()\n"; delete p; }

    void foo() override { cout << "Derived::foo()\n"; }

    vector<int> v;
    int *p;
};

int main1() {
    //
    // Метод - это функция, которая принимает первым аргументом указатель на экземпляр.
    // Полиморфизм - способность функции обрабатывать данные разных типов.
    //
    // Если у класса есть есть виртуальный метод, то такой класс называется полиморфным.
    // Свойство полиморфности наследуется от родителя к потомку.
    // Свойство виртуальности наследуется от метода родителя к методу потомку.
    // (полиморфность класса и виртуальность функции, по сути, - генетическая мутация, которая передаётся из поколения в поколение)
    //
    // Лучше самому потрогать и почитать https://en.cppreference.com/w/cpp/language/virtual.
    //
    // Порядок вызова конструктора:                          от родителей к потомку. Пример выше.
    // Порядок вызова деструктора:                           от потомка к родителям. Пример: `delete a;` и `delete d;`, объяснение в конце.
    // Порядок виртуального вызова виртуального деструктора: от потомков к родителям.
    //
    Derived *d = new Derived; // Вызовется `Base::Base()` и `Derived::Derived()`.
    Base *a = dynamic_cast<Base *>(d);
    Base *b = new Base();
    delete b;

    a->bar(); // - вызов метода.
              // Компилятор внутренне вызовет функцию `Base::bar(a, ...)`.

    a->foo(); // - виртуальный вызов виртуального метода.
              // Компилятор интерпретирует это как `a->vptr[method_index](...)`, и внутренне вызовет функцию `Derived::foo(a, ...)`.
              // У каждого полиморфного класса есть указатель `vptr` на тип `vtable`, который содержит виртуальные функции.
              //
              // Виртуальный вызов виртуальной функции - это вызов виртуальной функции динамического типа потомка.
              // Если у статического класса (`Base`) экземпляра (`*a`) нет свойства полиморфности, то динамический тип равен статическому типу объекта.
              // Динамический тип `*d` - `Derived`, статический тип `*d` - `Derived`.
              // Динамический тип `*a` - `Derived`, статический тип `*a` - `Base`.

    a->Base::foo(); // - вызов метода по имени.
                    // Компилятор внутренне вызовет функцию `Base::foo(&a, ...)`
                    // По имени можно вызвать виртуальный и не виртуальный методы.

    // Как можно получить утечку:
    // - Не освободив ресурсы, в том числе в деструкторе.
    // - Если в конструкторе динамического типа объекта есть выделение памяти, даже если используются умные указатели.
    // - Потерять указатель.
    // - Если в конструкторе произошло исключение. Нет конструктора - нет и деструктора. Решение: используем умные указатели.
    // - И другими способами, связанные с UB.
    //
    // Так же, утечку можно получить, если в конструкторе произошло исключение. Нет конструктора - нет и деструктора. Решение: используем умные указатели.
    // Попробуем освобождить место, что бы утечек не было:

    // delete a; // Во-первых, деструктор динамического типа экземпляра `a` НЕ виртуальный, то не будет виртуального вызова виртуального деструктора. Следовательно, возможна утечка.
                 // Во-вторых, это UB, о нём ниже.
    delete d; // Тут получаем красивый варнинг "Delete called on non-final 'Derived' that has virtual functions but non-virtual destructor"
              // Теперь заметка на https://en.cppreference.com/w/cpp/language/virtual:
              // - Если класс экземпляра полиморфный (объявляет виртуальный метод или наследует полиморфность), и его деструктор не виртуальный, то, удаляя объект, мы получим UB.
              // Но с одним уточнением на https://en.cppreference.com/w/cpp/memory/new/operator_delete:
              // - Если статический тип удаляемого объекта отличается от динамического типа (например, при удалении полиморфного объекта через указатель на базу),
              //   и если деструктор в статическом типе является виртуальным, то форма удаления одиночного объекта начинает поиск имени функции освобождения,
              //   начиная с точки определения окончательного переопределения ее виртуального деструктора. Независимо от того, какая функция освобождения
              //   будет выполняться во время выполнения, статически видимая версия оператора delete должна быть доступна для компиляции.
              //   В других случаях при удалении массива с помощью указателя на базу или при удалении с помощью указателя на базу с не виртуальным деструктором, поведение не определено.
              // Так как тут статический тип соотвествует динамическому, поэтому весь подпункт 3 пункта 8.5.2.5 действующего стандарта не учитывается и это не UB.
              // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
              // - In a single-object delete expression, if the static type of the object to be deleted is different from its dynamic
              // type, the static type shall be a base class of the dynamic type of the object to be deleted and the static type
              // shall have a virtual destructor or the behavior is undefined. In an array delete expression, if the dynamic
              // type of the object to be deleted differs from its static type, the behavior is undefined.
              // Я заебался и хуй знает как это интерпретировать. Виртуальные деструкторы нужны и точка. Наследоваться от них - нахуй не нужно.
    cout<<"-\n";
    return 0;
}




class VBase {
public:
    VBase() { cout << "VBase::VBase()\n"; }

    virtual ~VBase() { cout << "VBase::~VBase()\n"; }

    // void foo() { cout << "VBase::foo()\n"; } // TODO: clion bug

    void foo(int some_value) { cout << "VBase::foo(int)\n"; }
};

class VDer : public VBase {
public:
    VDer() { cout << "VDer::VDer()\n"; }

    ~VDer() override { cout << "VDer::~VDer()\n"; }

    virtual void foo() { cout << "VDer::foo()\n"; }
    // Тут будет предупреждение: Function 'foo' hides a non-virtual function from class 'VBase'.
    // Во-первых, предупреждение напоминает о том, что если у экземпляра статический тип != динамический тип, и я попытаюсь вызвать метод `foo()`, то будет НЕ виртуальный вызов (будет обычный вызов).
    // Во-вторых, если я попытаюсь вызвать `b.foo(int)`, то вылезет ошибка, т.к. я спрятал `VBase::foo(int)`.
    // Решение: `b.VBase::foo(int)`.
};

class VDerDer : public VDer {
public:
    VDerDer() { cout << "VDerDer::VDerDer()\n"; }

    ~VDerDer() override { cout << "VDerDer::~VDerDer()\n"; }

    // void fu() override { cout << "VDerDer::foo()\n"; } // Хорошая практика: override specifier помогает в compile-time найти ошибку в логике программиста или опечатку.
                                                          // Например, когда мы поменяли сигнатуру родительского класса.

    void foo() override { cout << "VDerDer::foo()\n"; }
};


int main2() {
    if (1) {
        VDer d; // Вызов `VDer::VDer()`, который внутри себя вызывает конструктор родителя.
                // Вывод: `VBase::VBase()` и `VDer::VDer()`.
        d.foo(); // Виртуальный вызов `VDer::foo()`.
        // d.foo(1); // Вызов `VBase::foo(int)`. error: too many arguments to function call, expected 0, have 1; did you mean 'VBase::foo'?
                     // Переопределяя НЕ виртуальный метод, я прячу базовый метод.
        d.VBase::foo(1); // Вызов `VBase::foo(int)`.
    } // Виртуальный вызов `VDer::~VDer()`. вывод: `VDer::~VDer()` и `VBase::~VBase()`.
      // Виртуальный вызов виртуального деструктора вызовет виртуальный деструктор динамического типа.
      // В отличии от предыдущего примера `main1()`, тут нет UB.
    cout<<"\n";

    if (1) {
        VDerDer dd; // Вызов `VDerDer::VDerDer()`, который внутри себя вызывает конструктор родителя.
        dd.foo(); // Виртуальный вызов `VDer::foo()`.
        dd.VBase::foo(2); // Вызов `VBase::foo()`
    } // Виртуальный вызов `VDerDer::~VDerived()`.
    cout<<"\n";

    if (1) {
        VDerDer dd;
        VBase &b = dd;
        // b.foo(); // Вызов `VBase::foo()`, которого НЕ существует, потому что вызов НЕ виртуальный и варнинги нужно читать всегда.
        // b.VDer::foo(); // 'VDer::foo' is not a member of class 'VBase'
    }
    cout<<"\n";

    if (1) {
        VDerDer dd;
        VDer &d = dd;
        d.foo(); // Виртуальный вызов `VDerDer::foo()`.
    }
    cout<<"\n";

    // Ещё приколы:
    // - https://stackoverflow.com/questions/52803895/overload-operator-in-template-base-class
    // - https://stackoverflow.com/questions/26714446/diamond-issue-with-virtual-function-implementation
    cout<<"-\n";
    return 0;
}

int main() {
    if (int rc = main1()) return rc;
    // if (int rc = main2()) return rc;
}
// 2. Виртуальное наследование TODO     Если деструктор удаляет данные, то тут функции, одни перекрывают другие и позволяют не дублироваться
//   Лучше почитать https://en.cppreference.com/w/cpp/language/derived_class (это другая ссылка)
//   Пример:
//   struct b {int a;}; // sizeof(b) == 4;
//   struct db1 : b {int b;}; // sizeof(b) == 12; потому что потому. TODO примеры
//   struct ddb1 : d1 {}; // sizeof(b) == 8;
//   struct db2 : virtual b {}; // sizeof(b) == 4;
