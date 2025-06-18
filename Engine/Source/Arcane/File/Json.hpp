#pragma once

#include <Arcane/Core.hpp>

#include <rapidjson/document.h>

namespace Arcane {

	typedef rapidjson::Document JsonDocument;
	typedef rapidjson::GenericValue<rapidjson::UTF8<>> JsonValue;
	typedef rapidjson::GenericObject<false, JsonValue> JsonObject;
	typedef rapidjson::GenericObject<true, JsonValue> JsonCObject;
	typedef rapidjson::GenericArray<false, JsonValue> JsonArray;
	typedef rapidjson::GenericArray<true, JsonValue> JsonCArray;

	JsonDocument ParseJson(const char *json) {
		JsonDocument doc;
		doc.Parse(json);

		AR_ASSERT(!doc.HasParseError(), "Could not parse json: {}\n", (uint32_t)doc.GetParseError());

		return doc;
	}

}