/****************************************************************************
*
*    Copyright (c) 2022 by Rockchip Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Rockchip Corporation. This is proprietary information owned by
*    Rockchip Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Rockchip Corporation.
*
*****************************************************************************/

#ifndef __ROCKIVA_BA_API_H__
#define __ROCKIVA_BA_API_H__

#include "rockiva_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ROCKIVA_BA_MAX_RULE_NUM          (16)                 /* 业务中最多规则数量 */

/* ---------------------------规则配置----------------------------------- */
/* 周界规则类型（绊线/区域事件）*/
typedef enum {
    ROCKIVA_BA_TRIP_EVENT_BOTH = 0,                  /* 拌线:双向触发 */
    ROCKIVA_BA_TRIP_EVENT_DEASIL = 1,                /* 拌线:顺时针触发 */
    ROCKIVA_BA_TRIP_EVENT_WIDDERSHINES = 2,          /* 拌线:逆时针触发 */
    ROCKIVA_BA_TRIP_EVENT_IN = 3,                    /* 进入区域[废弃] */
    ROCKIVA_BA_TRIP_EVENT_OUT = 4,                   /* 离开区域[废弃] */
    ROCKIVA_BA_TRIP_EVENT_STAY = 5,                  /* 区域入侵[废弃] */
} RockIvaBaTripEvent;

/* 目标规则触发类型 */
typedef enum {
    ROCKIVA_BA_RULE_NONE = 0,
    ROCKIVA_BA_RULE_CROSS = 0x1,          /* 拌线 */
    ROCKIVA_BA_RULE_INAREA = 0x2,         /* 进入区域 */
    ROCKIVA_BA_RULE_OUTAREA = 0x4,        /* 离开区域 */
    ROCKIVA_BA_RULE_STAY = 0x8,           /* 区域入侵 */
} RockIvaBaRuleTriggerType;

/* 越界规则 */
typedef struct {
    uint8_t ruleEnable;                         /* 规则是否启用，1->启用，0->不启用 */
    uint32_t ruleID;                            /* 规则ID */
    RockIvaLine line;                           /* 越界线配置 */
    RockIvaLine directLine;                     /* 方向线配置[废弃] */
    RockIvaBaTripEvent event;                   /* 越界方向 */
    RockIvaSize minObjSize[ROCKIVA_OBJECT_TYPE_MAX];                  /* 万分比表示 最小目标 */
    RockIvaSize maxObjSize[ROCKIVA_OBJECT_TYPE_MAX];                  /* 万分比表示 最大目标 */
    uint8_t scores[ROCKIVA_OBJECT_TYPE_MAX];    /* 各类别过滤分数阈值，0为内部自动 */
    uint32_t objType;                           /* 配置触发目标类型 */  
    uint8_t rulePriority;                       /* 规则优先级： 0 高， 1 中， 2 低 */
    uint8_t sense;                              /* 灵敏度,1~100 */
    uint8_t trigerMode;                         /* 触发模式： 0： 目标仅触发一次；1： 目标每次越界都触发 */
} RockIvaBaWireRule;

/* 区域规则 */
typedef struct {
    uint8_t ruleEnable;                         /* 规则是否启用，1->启用，0->不启用 */
    uint32_t ruleID;                            /* 规则ID */
    RockIvaArea area;                           /* 区域配置 */
    RockIvaBaTripEvent event;                   /* 区域事件[废弃] */
    RockIvaSize minObjSize[ROCKIVA_OBJECT_TYPE_MAX];                  /* 万分比表示 最小目标 */
    RockIvaSize maxObjSize[ROCKIVA_OBJECT_TYPE_MAX];                  /* 万分比表示 最大目标 */
    uint8_t scores[ROCKIVA_OBJECT_TYPE_MAX];    /* 各类别过滤分数阈值，0为内部自动 */
    uint32_t objType;                           /* 配置触发目标类型 */
    uint32_t alertTime;                         /* 告警时间设置 */
    uint8_t sense;                              /* 灵敏度,1~100 */
    uint8_t checkEnter;                         /* 区域入侵规则是否需要检查目标有进入　[0: 不启用，１：启用]　*/
} RockIvaBaAreaRule;

/* 行为分析类规则配置 */
typedef struct {
    RockIvaBaWireRule tripWireRule[ROCKIVA_BA_MAX_RULE_NUM];            /* 越界事件 */
    RockIvaBaAreaRule areaInRule[ROCKIVA_BA_MAX_RULE_NUM];              /* 进入区域 */
    RockIvaBaAreaRule areaOutRule[ROCKIVA_BA_MAX_RULE_NUM];             /* 离开区域 */
    RockIvaBaAreaRule areaInBreakRule[ROCKIVA_BA_MAX_RULE_NUM];         /* 区域入侵 */
} RockIvaBaTaskRule;

typedef struct {
    uint8_t filterPersonMode;           /* 过滤非机动车/机动车驾驶员人形结果 0: 不过滤; 1: 过滤 */
    uint8_t detectResultMode;           /* 上报目标检测结果模式 0: 不上报没有触发规则的检测目标; 1: 上报没有触发规则的检测目标 */
} RockIvaBaAiConfig;

/* 行为分析业务初始化参数配置 */
typedef struct {
    RockIvaBaTaskRule baRules;                    /* 行为分析规则参数配置初始化 */
    RockIvaBaAiConfig aiConfig;                   /* 算法配置 */
} RockIvaBaTaskParams;

/* ------------------------------------------------------------------ */

/* -------------------------- 算法处理结果 --------------------------- */

/* 第一次触发规则信息 */
typedef struct {
    int32_t ruleID;                             /* 触发规则ID */
    RockIvaBaRuleTriggerType triggerType;       /* 触发规则类型 */
} RockIvaBaTrigger;

/* 触发周界规则的单个目标基本信息 */
typedef struct {
    RockIvaObjectInfo objInfo;                          /* 目标信息 */
    uint32_t triggerRules;                              /* 目标触发规则 */
    uint32_t triggerRulesNum;                           /* 目标触发规则数量 */
    uint32_t triggerRulesID[ROCKIVA_BA_MAX_RULE_NUM];   /* 目标触发的所有规则ID */
    RockIvaBaTrigger firstTrigger;                      /* 目标第一次触发的规则,可用于抓拍 */
} RockIvaBaObjectInfo;

/* 检测结果全部信息 */
typedef struct {
    uint32_t frameId;                                         /* 输入图像帧ID */
    RockIvaImage frame;                                       /* 对应的输入图像帧 */
    uint32_t channelId;                                       /* 通道号 */
    uint32_t objNum;                                          /* 目标个数 */
    RockIvaBaObjectInfo triggerObjects[ROCKIVA_MAX_OBJ_NUM];  /* 触发周界规则的目标 */
} RockIvaBaResult;

/* ---------------------------------------------------------------- */

/**
 * @brief 周界结果回调函数
 *
 * result 结果
 * status 状态码
 * userdata 用户自定义数据
 */
typedef void (*ROCKIVA_BA_ResultCallback)(const RockIvaBaResult* result, const RockIvaExecuteStatus status,
                                          void* userdata);

/**
 * @brief 初始化
 *
 * @param handle [INOUT] 要初始化的handle
 * @param initParams [IN] 初始化参数
 * @param resultCallback [IN] 回调函数
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_BA_Init(RockIvaHandle handle, const RockIvaBaTaskParams* params,
                               const ROCKIVA_BA_ResultCallback resultCallback);

/**
 * @brief 运行时重新配置(重新配置会导致内部的一些记录清空复位，但是模型不会重新初始化)
 * 
 * @param handle [IN] handle
 * @param initParams [IN] 配置参数
 * @return RockIvaRetCode 
 */
RockIvaRetCode ROCKIVA_BA_Reset(RockIvaHandle handle, const RockIvaBaTaskParams* params);

/**
 * @brief 释放
 *
 * @param handle [in] handle
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_BA_Release(RockIvaHandle handle);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of #ifndef __ROCKIVA_BA_API_H__ */