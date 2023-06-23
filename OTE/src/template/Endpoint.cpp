//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Endpoint.h"

#include "src/template/Extractor.h"
#include "src/template/Template.h"

OTE::Endpoint::Endpoint(OTE::Template *_tmplt):
    tmplt(_tmplt)
{}

OTE::Endpoint::~Endpoint()
{
    foreach(Extractor *extractor, extractors)
        delete extractor;
}
