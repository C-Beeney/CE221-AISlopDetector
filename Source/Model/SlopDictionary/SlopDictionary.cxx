/*---------------------------------------------------------------------------|
|                                                                            |
|   Copyright 2026 Connor R Beeney                                           |
|                                                                            |
|   Licensed under the Apache License, Version 2.0 (the "License");          |
|   you may not use this file except in compliance with the License.         |
|   You may obtain a copy of the License at                                  |
|                                                                            |
|       http://www.apache.org/licenses/LICENSE-2.0                           |
|                                                                            |
|   Unless required by applicable law or agreed to in writing, software      |
|   distributed under the License is distributed on an "AS IS" BASIS,        |
|   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
|   See the License for the specific language governing permissions and      |
|   limitations under the License.                                           |
|                                                                            |
|---------------------------------------------------------------------------*/

#include <Model/SlopDictionary.hxx>

#include "SlopDictionary.DiskUtil.hxx"

#include <cstring>
#include <new>
#include <cassert>

using ::Model::SlopDictionary;
using enum SlopDictionary::Error;
typedef ::Model::SlopDictionary::Mode Mode;

SlopDictionary::SlopDictionary(
	const Mode mode,
	const std::optional<std::string_view> filePathOption
)	: filePath(nullptr)
	, entries{}
	, mode(mode)
{
	if (!filePathOption) {
		assert(Mode::MEM == this->mode);
		return;
	}

	{
		auto filePath = filePathOption.value();
		auto buffer = new char[filePath.length() + 1];
		buffer[filePath.length()] = '\0';
		memcpy(buffer, filePath.data(), filePath.length());
		this->filePath = buffer;
	}

	this->prop = { {"entries", std::ref(this->entries)} };

	auto rc = SlopDictionaryDiskUtil::load(this->filePath, this->entries);
	assert(rc == SlopDictionaryDiskUtil::Error::OK); /* TEMP: CHANGE! */
}

SlopDictionary::~SlopDictionary( void
) {
	if(Mode::MEM != this->mode) {
		assert(this->filePath);
		SlopDictionaryDiskUtil::save(this->filePath, this->entries);
	}

	for(auto i = 0; i < this->ENTRY_MAX; ++i) {
		delete[] this->entries[i];
	}

	delete[] this->filePath;
}

static SlopDictionary::Error
sync(
	const char *const filePath,
	const char *const (&entries)[::Model::SlopDictionary::ENTRY_MAX]
) {
	SlopDictionary::Error rc = OK;
	using DiskError = SlopDictionaryDiskUtil::Error;
	const auto ec = SlopDictionaryDiskUtil::save(filePath, entries);
	switch (ec) {
	case DiskError::OK      : rc = OK;         break;
	case DiskError::TOOLONG : [[fallthrough]];
	case DiskError::INVALID : [[fallthrough]];
	case DiskError::TOOMANY : rc = ILLEGAL;    break;
	case DiskError::NOMEM   : rc = NOMEM;      break;
	case DiskError::IO      : rc = IO;         break;
	}
	return rc;
}

SlopDictionary::Error
SlopDictionary::add(
        const std::string_view string
) {
	const char **it = this->entries;
	for(; it - this->entries < ENTRY_MAX && *it; ++it);

	if(ENTRY_MAX == it - this->entries) {
		return FULL;
	}

	const unsigned short length = string.length();
	auto buffer = new (std::nothrow) char[length + 1];

	if(nullptr == buffer) {
		return NOMEM;
	}

	memset(buffer, 0, length + 1);
	memcpy(buffer, string.data(), length);
	*it = buffer;
	
	Error rc = OK;
	if(Mode::SYNC == this->mode) {
		rc = sync(this->filePath, this->entries);
	}

	return rc;
}

SlopDictionary::Error
SlopDictionary::replace(
        const std::string_view string,
        const unsigned short index
) {
	using std::memset, std::memcpy, std::nothrow;
	
	if(index >= this->ENTRY_MAX) {
		return RANGE;
	}
	auto &entry = this->entries[index];

	if(nullptr == entry) {
		return EMPTY;
	}

	const unsigned short length = string.length();
	auto buffer = new (nothrow) char[length + 1];
	
	if(nullptr == buffer) {
		return NOMEM;
	}
	
	memset(buffer, 0, length + 1);
	memcpy(buffer, string.data(), length);

	delete[] entry;
	entry = buffer;

	Error rc = OK;
	if(Mode::SYNC == this->mode) {
		rc = sync(this->filePath, this->entries);
	}

	return rc;
}

SlopDictionary::Error
SlopDictionary::remove(
	const unsigned short index
) {
	if(index >= ENTRY_MAX) {
		return RANGE;
	}

	if(nullptr == this->entries[index]) {
		return EMPTY;
	}

	delete[] this->entries[index];
	this->entries[index] = nullptr;

	Error rc = OK;
	if(Mode::SYNC == this->mode) {
		rc = sync(this->filePath, this->entries);
	}

	return rc;
}

void
SlopDictionary::setMode(
	const Mode newMode
) {
	this->mode = newMode;
}

Mode
SlopDictionary::getMode( void
) const {
	return this->mode;
}

using GetEntriesRet = const char *const (&)[SlopDictionary::ENTRY_MAX];

GetEntriesRet
SlopDictionary::getEntries( void
) const {
	return this->entries;
}

const char *const&
SlopDictionary::operator[](
	unsigned long i
) const {
	return this->getEntries()[i];
}