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

#include <filesystem>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <new>

#include <iostream>
#include <fstream>

using namespace SlopDictionaryDiskUtil;

using LoadDictType =
        const char * (&)[::Model::SlopDictionary::ENTRY_MAX];
using SaveDictType =
        const char *const (&)[::Model::SlopDictionary::ENTRY_MAX];


static void free_entries(char **entries, const unsigned count) {
        for (unsigned i = 0; i < count; delete[] entries[i++]);
}

Error
SlopDictionaryDiskUtil::load(
        const char *const path,
        LoadDictType dict,
        const unsigned count
) {
	using enum Error;

	constexpr int LINE_LENGTH_MAX = 0xFF;

	FILE *fp;
	char line[LINE_LENGTH_MAX + 2] {};
	unsigned counter = 0;
	Error rc = OK;
	char **temp;

	if (not path || count != ::Model::SlopDictionary::ENTRY_MAX) {
		return INVALID;
	}

	for (unsigned i = 0; i < count; ++i) {
		if (dict[i] != nullptr) {
			return INVALID;
		}
	}

	if (not (fp = fopen(path, "rb"))) {
		return IO;
	}

	if (not (temp = new (std::nothrow) char*[count] {})) {
		rc = NOMEM;
		goto exit;
	}

	for(counter = 0; counter < count; ++counter) {
		char* entry = fgets(line, sizeof line, fp);

		if (nullptr == entry) {
			if (ferror(fp)) {
			rc = IO;
			goto fail;
			}
			break;
		}

		unsigned entry_length = strlen(entry);

		if (entry_length && entry[entry_length - 1] == '\n') {
			entry[--entry_length] = '\0';

			if (entry_length && entry[entry_length - 1] == '\r') {
			entry[--entry_length] = '\0';
			}
		}

		if (entry_length >= LINE_LENGTH_MAX) {
			rc = TOOLONG;
			goto fail;
		}

		temp[counter] = new (std::nothrow) char[entry_length + 1];
		if (nullptr == temp[counter]) {
			rc = NOMEM;
			goto fail;
		}

		memcpy(temp[counter], entry, entry_length + 1);
	}

	if (0 != fread(line, 1, 1, fp)) {
		rc = TOOMANY;
		goto fail;
	}

	if (ferror(fp)) {
		rc = IO;
		goto fail;
	}

	if (fclose(fp)) {
		fp = nullptr;
		rc = IO;
		goto fail;
	}
	fp = nullptr;

	for (unsigned i = 0; i < counter; ++i) {
		dict[i] = temp[i];
		temp[i] = nullptr;
	}

fail:
	free_entries(temp, count);

exit:
	delete[] temp;
	if(fp && fclose(fp) && rc == OK) rc = IO;
	return rc;

}

Error
SlopDictionaryDiskUtil::save(
    const char* const path,
    SaveDictType dict
) {
	using enum Error;

	constexpr long ENTRY_MAX =
		::Model::SlopDictionary::ENTRY_MAX;
	constexpr long LINE_LENGTH_MAX = 0xFF;
	constexpr char temp_suffix[] = ".tmp";

	if (!path || !*path) {
		return INVALID;
	}

	bool gap = false;

	for (unsigned i = 0; i < ENTRY_MAX; ++i) {
		if (!dict[i]) {
			gap = true;
			continue;
		}

		if (gap) {
			return INVALID;
		}

		const unsigned length = strlen(dict[i]);

		if (length >= LINE_LENGTH_MAX) {
			return TOOLONG;
		}

		if (strchr(dict[i], '\n') ||
			(length && dict[i][length - 1] == '\r')) {
			return INVALID;
		}
	}

	const unsigned path_length = strlen(path);

	char* temp_path = new (std::nothrow)
	char[path_length + sizeof temp_suffix];

	if (!temp_path) {
		return NOMEM;
	}

	memcpy(temp_path, path, path_length);
	memcpy(
		temp_path + path_length,
		temp_suffix,
		sizeof temp_suffix
	);

	FILE* fp = fopen(temp_path, "wb");

	if (!fp) {
		delete[] temp_path;
		return IO;
	}

	Error rc = OK;

	for (unsigned i = 0; i < ENTRY_MAX && dict[i]; ++i) {
		const unsigned length = strlen(dict[i]);

		if (fwrite(dict[i], 1, length, fp) != length ||
			fputc('\n', fp) == EOF) {
			rc = IO;
			break;
		}
	}

	if (fclose(fp) != 0) {
		rc = IO;
	}

	if (rc == OK && rename(temp_path, path) != 0) {
		rc = IO;
	}

	if (rc != OK) {
		(void)remove(temp_path);
	}

	delete[] temp_path;

	return rc;
}
