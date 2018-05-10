#include "test.hpp"
#include "file/json.hpp"
#include "util/timer.hpp"


void Base::TestApp::testJSON()
{
    cout << "testJSON" << endl;

    // Loading and saving
    try
    {
        Timer t1("JSON load");
        JsonFile jf(((TextFile*)resHandler->get("hello.json"))->getText());
        t1.stopAndPrint();

        Timer t2("JSON save");
        jf.save(FilePath("C:/Dev/Builds/base/out.json"));
        t2.stopAndPrint();

        cout << "foo: " << jf.getString("foo") << endl;
        cout << "Bar: " << jf.getNumber("Bar") << endl;

        JsonArray* arr =  jf.getArray("Array");
        cout << "Array length: " << arr->getCount() << endl;
        cout << " 0: " << arr->getNumber(0) << endl;
        cout << " 1: " << arr->getNumber(1) << endl;
        cout << " 2: " << arr->getString(2) << endl;
        cout << " 2 isNull: " << (arr->isNull(2) ? "true": "false") << endl;

        JsonObject* obj =  jf.getArray("Objects")->getObject(0);
        cout << "Object int: " << obj->getNumber("int") << endl;
        cout << "Object float: " << obj->getNumber("float") << endl;
        cout << "Object str: " << obj->getString("str") << endl;
        cout << "Object multilinestr: " << obj->getString("multilinestr") << endl;
        cout << "Object multilinestr isNull: " << (obj->isNull("multilinestr") ? "true": "false") << endl;
        cout << "Object null isNull: " << (obj->isNull("null") ? "true": "false") << endl;
        cout << "Object bool: " << (obj->getBool("bool") ? "true": "false") << endl;
        //JsonObject* nonExistant =  jf.getArray("Objects")->getObject(1);

        jf.save(FilePath("C:/Dev/Builds/base/out1.json"));
    }
    catch (const JsonException& ex)
    {
        cout << ex.what() << endl;
    }

    // Generating
    try
    {
        Timer t1("JSON generate");
        JsonFile jf;
        JsonArray* arr = jf.addArray("elements");
        repeat (5)
        {
            JsonObject* obj = arr->addObject();
            obj->addString("name", "HelloWorld");
            obj->addNumber("value", 14052);
            obj->addObject("sub")->addNumber("cost", 100);
            obj->addNull("null example");
        }
        t1.stopAndPrint();

        Timer t2("JSON save");
        jf.save(FilePath("C:/Dev/Builds/base/out2.json"));
        t2.stopAndPrint();
    }
    catch (const JsonException& ex)
    {
        cout << ex.what() << endl;
    }
    
    cout << std::flush;
}