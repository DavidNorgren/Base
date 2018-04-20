#pragma once

#include "file/json.hpp"
#include "util/timer.hpp"

namespace Base
{
    void testJSON(ResourceHandler* resHandler)
    {
        cout << "testJSON" << endl;
        
        // Loading and saving
        try
        {
            Timer t1("JSON load");
            //JsonFile jf(resHandler->find("hello.json"));
            JsonFile jf("D:/Dropbox/Projects/Minecraft/Mine-imator/Models/Custom/Snowball.json");
            t1.stopAndPrint();

            cout << jf.getString("credit") << endl;
            cout << jf.getArray("elements")->values.size() << endl;

            Timer t2("JSON save");
            jf.save("C:/Dev/Builds/base/out.json");
            t2.stopAndPrint();

            cout << "foo: " << jf.getString("foo") << endl;
            cout << "Bar: " << jf.getNumber("Bar") << endl;

            JsonArray* arr =  jf.getArray("Array");
            cout << "Array length: " << arr->values.size() << endl;
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

            jf.save("C:/Dev/Builds/base/out.json");
        }
        catch (JsonException& ex)
        {
            cout << ex.what() << endl;
        }
    }
}