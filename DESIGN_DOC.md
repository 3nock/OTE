# SpiderSuite Design Document

The design document for the SpiderSuite project, contains the goals and proposed solutions. 

## Overview

This project aims at developing a modern and feature full web crawler/spider specially designed for cyber security professionals.

`Status:` In progress

## Stakeholders

- [Enock N. Michael](https://github.com/3nock) (Main developer)

## Goals

Building an advance web crawler/spider.

- Building a fast, scallable and redundant spider
- Building a highly interactive and user friendly graphical user interface (GUI)
- Developing a graph feature to view the crawled pages
- Designing an efficient database system
- Developing analysis tools and modules to analyze different web pages and filetypes
- Designing and developing a plugin system and api to help other developer create plugins for the 3Spider tool.
- Exntensive testing and developing test cases for each and every feature.
- Developing a dynamic crawler/spider for crawling javascript heavy webpages.
- Developing a destributed crawling system 

## Solutions

Proposed solutions for some of the determined goals:

- Using google test for testing each and every feature in 3Spider project, testing even third party modules.
- Using graphviz for visualization of the crawled pages.
- Using sqlite3 as a relative database management system. 
Reasons:
    - No Installation required
    - Has a good C API
- Use Qt QwebEngine (chromium) for dynamic crawling 

## Planned Features

Modules to be developed for 3Spider program:

- Wappalyzer, for detecting technologies used to develop the webpage
- SEO webpage ranking module, to analyze webpage, identifying pitfalls and ranking them (use google pagerank algorithm.
- Passive Crawling system. Module for passively  obtaining relevant links using publicly available resources e.g waybackmachine api.
- Bruteforce module. For identification of webpages by bruteforcing.
- Plugins for other similar tools such as burp suite, zap proxy, fiddler. For importing 3Spider results to them and export thier results to 3Spider.

## Timeline
- Internationalization of the source code
- Testing
