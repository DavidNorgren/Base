#pragma once

#include "file/json.hpp"


namespace Base
{
    void testJSON(ResourceHandler* resHandler)
    {
        cout << "testJSON" << endl;
        
        // Loading and saving
        try
        {
            Base::JsonFile jf(resHandler->find("hello.json"));

            cout << "foo: " << jf.getString("foo") << endl;
            cout << "Bar: " << jf.getNumber("Bar") << endl;

            Base::JsonArray* arr =  jf.getArray("Array");
            cout << "Array length: " << arr->values.size() << endl;
            cout << " 0: " << arr->getNumber(0) << endl;
            cout << " 1: " << arr->getNumber(1) << endl;
            cout << " 2: " << arr->getString(2) << endl;
            cout << " 2 isNull: " << (arr->isNull(2) ? "true": "false") << endl;

            Base::JsonObject* obj =  jf.getArray("Objects")->getObject(0);
            cout << "Object int: " << obj->getNumber("int") << endl;
            cout << "Object float: " << obj->getNumber("float") << endl;
            cout << "Object str: " << obj->getString("str") << endl;
            cout << "Object multilinestr: " << obj->getString("multilinestr") << endl;
            cout << "Object multilinestr isNull: " << (obj->isNull("multilinestr") ? "true": "false") << endl;
            cout << "Object null isNull: " << (obj->isNull("null") ? "true": "false") << endl;
            cout << "Object bool: " << (obj->getBool("bool") ? "true": "false") << endl;
            //Base::JsonObject* nonExistant =  jf.getArray("Objects")->getObject(1);

            jf.save("C:/Dev/Builds/base/out.json");
        }
        catch (Base::JsonException& ex)
        {
            cout << ex.what() << endl;
        }
    }
}