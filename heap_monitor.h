/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   heap_monitor.h
 * Author: Adam
 *
 * Created on Streda, 2017, decembra 6, 14:47
 */
#pragma once

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
// #include <crtdbg.h> funguje iba na visualku

#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define initHeapMonitor() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

