/*
 * dds.c
 *
 *  Created on: Mar 13, 2022
 *      Author: 王志涵
 */

#include "dds.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "main.h"
#include "cmd_process.h"
#include "arm_math.h"
#include "arm_common_tables.h"

extern DMA_HandleTypeDef hdma_dac1;
float32_t offset[_Size/2]={
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,
		2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048
};
float32_t magChgPort0[_Size/2];
float32_t magChgPort1[_Size/2];
uint16_t sinWave[_Size]={
2048,2054,2060,2066,2073,2079,2085,2091,2098,2104,2110,2117,2123,2129,2135,2142,
2148,2154,2161,2167,2173,2179,2186,2192,2198,2204,2211,2217,2223,2229,2236,2242,
2248,2254,2261,2267,2273,2279,2286,2292,2298,2304,2311,2317,2323,2329,2336,2342,
2348,2354,2360,2367,2373,2379,2385,2391,2398,2404,2410,2416,2422,2429,2435,2441,
2447,2453,2459,2466,2472,2478,2484,2490,2496,2502,2508,2515,2521,2527,2533,2539,
2545,2551,2557,2563,2569,2576,2582,2588,2594,2600,2606,2612,2618,2624,2630,2636,
2642,2648,2654,2660,2666,2672,2678,2684,2690,2696,2702,2708,2714,2720,2726,2732,
2737,2743,2749,2755,2761,2767,2773,2779,2785,2790,2796,2802,2808,2814,2820,2825,
2831,2837,2843,2849,2854,2860,2866,2872,2877,2883,2889,2895,2900,2906,2912,2917,
2923,2929,2934,2940,2946,2951,2957,2963,2968,2974,2980,2985,2991,2996,3002,3007,
3013,3018,3024,3030,3035,3041,3046,3051,3057,3062,3068,3073,3079,3084,3090,3095,
3100,3106,3111,3117,3122,3127,3133,3138,3143,3148,3154,3159,3164,3170,3175,3180,
3185,3191,3196,3201,3206,3211,3216,3222,3227,3232,3237,3242,3247,3252,3257,3262,
3267,3273,3278,3283,3288,3293,3298,3303,3307,3312,3317,3322,3327,3332,3337,3342,
3347,3352,3356,3361,3366,3371,3376,3380,3385,3390,3395,3399,3404,3409,3414,3418,
3423,3428,3432,3437,3441,3446,3451,3455,3460,3464,3469,3473,3478,3482,3487,3491,
3496,3500,3505,3509,3513,3518,3522,3526,3531,3535,3539,3544,3548,3552,3557,3561,
3565,3569,3573,3578,3582,3586,3590,3594,3598,3602,3606,3611,3615,3619,3623,3627,
3631,3635,3639,3643,3646,3650,3654,3658,3662,3666,3670,3674,3677,3681,3685,3689,
3692,3696,3700,3704,3707,3711,3715,3718,3722,3726,3729,3733,3736,3740,3743,3747,
3750,3754,3757,3761,3764,3768,3771,3774,3778,3781,3784,3788,3791,3794,3798,3801,
3804,3807,3811,3814,3817,3820,3823,3826,3829,3833,3836,3839,3842,3845,3848,3851,
3854,3857,3860,3862,3865,3868,3871,3874,3877,3880,3882,3885,3888,3891,3893,3896,
3899,3902,3904,3907,3909,3912,3915,3917,3920,3922,3925,3927,3930,3932,3935,3937,
3940,3942,3944,3947,3949,3951,3954,3956,3958,3961,3963,3965,3967,3969,3972,3974,
3976,3978,3980,3982,3984,3986,3988,3990,3992,3994,3996,3998,4000,4002,4004,4005,
4007,4009,4011,4013,4014,4016,4018,4020,4021,4023,4025,4026,4028,4029,4031,4033,
4034,4036,4037,4039,4040,4042,4043,4044,4046,4047,4048,4050,4051,4052,4054,4055,
4056,4057,4059,4060,4061,4062,4063,4064,4065,4066,4067,4068,4069,4070,4071,4072,
4073,4074,4075,4076,4077,4078,4079,4079,4080,4081,4082,4082,4083,4084,4084,4085,
4086,4086,4087,4087,4088,4088,4089,4089,4090,4090,4091,4091,4092,4092,4092,4093,
4093,4093,4094,4094,4094,4094,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,
4096,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4094,4094,4094,4094,4093,
4093,4093,4092,4092,4092,4091,4091,4090,4090,4089,4089,4088,4088,4087,4087,4086,
4086,4085,4084,4084,4083,4082,4082,4081,4080,4079,4079,4078,4077,4076,4075,4074,
4073,4072,4071,4070,4069,4068,4067,4066,4065,4064,4063,4062,4061,4060,4059,4057,
4056,4055,4054,4052,4051,4050,4048,4047,4046,4044,4043,4042,4040,4039,4037,4036,
4034,4033,4031,4029,4028,4026,4025,4023,4021,4020,4018,4016,4014,4013,4011,4009,
4007,4005,4004,4002,4000,3998,3996,3994,3992,3990,3988,3986,3984,3982,3980,3978,
3976,3974,3972,3969,3967,3965,3963,3961,3958,3956,3954,3951,3949,3947,3944,3942,
3940,3937,3935,3932,3930,3927,3925,3922,3920,3917,3915,3912,3909,3907,3904,3902,
3899,3896,3893,3891,3888,3885,3882,3880,3877,3874,3871,3868,3865,3862,3860,3857,
3854,3851,3848,3845,3842,3839,3836,3833,3829,3826,3823,3820,3817,3814,3811,3807,
3804,3801,3798,3794,3791,3788,3784,3781,3778,3774,3771,3768,3764,3761,3757,3754,
3750,3747,3743,3740,3736,3733,3729,3726,3722,3718,3715,3711,3707,3704,3700,3696,
3692,3689,3685,3681,3677,3674,3670,3666,3662,3658,3654,3650,3646,3643,3639,3635,
3631,3627,3623,3619,3615,3611,3606,3602,3598,3594,3590,3586,3582,3578,3573,3569,
3565,3561,3557,3552,3548,3544,3539,3535,3531,3526,3522,3518,3513,3509,3505,3500,
3496,3491,3487,3482,3478,3473,3469,3464,3460,3455,3451,3446,3441,3437,3432,3428,
3423,3418,3414,3409,3404,3399,3395,3390,3385,3380,3376,3371,3366,3361,3356,3352,
3347,3342,3337,3332,3327,3322,3317,3312,3307,3303,3298,3293,3288,3283,3278,3273,
3267,3262,3257,3252,3247,3242,3237,3232,3227,3222,3216,3211,3206,3201,3196,3191,
3185,3180,3175,3170,3164,3159,3154,3148,3143,3138,3133,3127,3122,3117,3111,3106,
3100,3095,3090,3084,3079,3073,3068,3062,3057,3051,3046,3041,3035,3030,3024,3018,
3013,3007,3002,2996,2991,2985,2980,2974,2968,2963,2957,2951,2946,2940,2934,2929,
2923,2917,2912,2906,2900,2895,2889,2883,2877,2872,2866,2860,2854,2849,2843,2837,
2831,2825,2820,2814,2808,2802,2796,2790,2785,2779,2773,2767,2761,2755,2749,2743,
2737,2732,2726,2720,2714,2708,2702,2696,2690,2684,2678,2672,2666,2660,2654,2648,
2642,2636,2630,2624,2618,2612,2606,2600,2594,2588,2582,2576,2569,2563,2557,2551,
2545,2539,2533,2527,2521,2515,2508,2502,2496,2490,2484,2478,2472,2466,2459,2453,
2447,2441,2435,2429,2422,2416,2410,2404,2398,2391,2385,2379,2373,2367,2360,2354,
2348,2342,2336,2329,2323,2317,2311,2304,2298,2292,2286,2279,2273,2267,2261,2254,
2248,2242,2236,2229,2223,2217,2211,2204,2198,2192,2186,2179,2173,2167,2161,2154,
2148,2142,2135,2129,2123,2117,2110,2104,2098,2091,2085,2079,2073,2066,2060,2054,
2048,2041,2035,2029,2022,2016,2010,2004,1997,1991,1985,1978,1972,1966,1960,1953,
1947,1941,1934,1928,1922,1916,1909,1903,1897,1891,1884,1878,1872,1866,1859,1853,
1847,1841,1834,1828,1822,1816,1809,1803,1797,1791,1784,1778,1772,1766,1759,1753,
1747,1741,1735,1728,1722,1716,1710,1704,1697,1691,1685,1679,1673,1666,1660,1654,
1648,1642,1636,1629,1623,1617,1611,1605,1599,1593,1587,1580,1574,1568,1562,1556,
1550,1544,1538,1532,1526,1519,1513,1507,1501,1495,1489,1483,1477,1471,1465,1459,
1453,1447,1441,1435,1429,1423,1417,1411,1405,1399,1393,1387,1381,1375,1369,1363,
1358,1352,1346,1340,1334,1328,1322,1316,1310,1305,1299,1293,1287,1281,1275,1270,
1264,1258,1252,1246,1241,1235,1229,1223,1218,1212,1206,1200,1195,1189,1183,1178,
1172,1166,1161,1155,1149,1144,1138,1132,1127,1121,1115,1110,1104,1099,1093,1088,
1082,1077,1071,1065,1060,1054,1049,1044,1038,1033,1027,1022,1016,1011,1005,1000,
995,989,984,978,973,968,962,957,952,947,941,936,931,925,920,915,
910,904,899,894,889,884,879,873,868,863,858,853,848,843,838,833,
828,822,817,812,807,802,797,792,788,783,778,773,768,763,758,753,
748,743,739,734,729,724,719,715,710,705,700,696,691,686,681,677,
672,667,663,658,654,649,644,640,635,631,626,622,617,613,608,604,
599,595,590,586,582,577,573,569,564,560,556,551,547,543,538,534,
530,526,522,517,513,509,505,501,497,493,489,484,480,476,472,468,
464,460,456,452,449,445,441,437,433,429,425,421,418,414,410,406,
403,399,395,391,388,384,380,377,373,369,366,362,359,355,352,348,
345,341,338,334,331,327,324,321,317,314,311,307,304,301,297,294,
291,288,284,281,278,275,272,269,266,262,259,256,253,250,247,244,
241,238,235,233,230,227,224,221,218,215,213,210,207,204,202,199,
196,193,191,188,186,183,180,178,175,173,170,168,165,163,160,158,
155,153,151,148,146,144,141,139,137,134,132,130,128,126,123,121,
119,117,115,113,111,109,107,105,103,101,99,97,95,93,91,90,
88,86,84,82,81,79,77,75,74,72,70,69,67,66,64,62,
61,59,58,56,55,53,52,51,49,48,47,45,44,43,41,40,
39,38,36,35,34,33,32,31,30,29,28,27,26,25,24,23,
22,21,20,19,18,17,16,16,15,14,13,13,12,11,11,10,
9,9,8,8,7,7,6,6,5,5,4,4,3,3,3,2,
2,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,
2,2,3,3,3,4,4,5,5,6,6,7,7,8,8,9,
9,10,11,11,12,13,13,14,15,16,16,17,18,19,20,21,
22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,38,
39,40,41,43,44,45,47,48,49,51,52,53,55,56,58,59,
61,62,64,66,67,69,70,72,74,75,77,79,81,82,84,86,
88,90,91,93,95,97,99,101,103,105,107,109,111,113,115,117,
119,121,123,126,128,130,132,134,137,139,141,144,146,148,151,153,
155,158,160,163,165,168,170,173,175,178,180,183,186,188,191,193,
196,199,202,204,207,210,213,215,218,221,224,227,230,233,235,238,
241,244,247,250,253,256,259,262,266,269,272,275,278,281,284,288,
291,294,297,301,304,307,311,314,317,321,324,327,331,334,338,341,
345,348,352,355,359,362,366,369,373,377,380,384,388,391,395,399,
403,406,410,414,418,421,425,429,433,437,441,445,449,452,456,460,
464,468,472,476,480,484,489,493,497,501,505,509,513,517,522,526,
530,534,538,543,547,551,556,560,564,569,573,577,582,586,590,595,
599,604,608,613,617,622,626,631,635,640,644,649,654,658,663,667,
672,677,681,686,691,696,700,705,710,715,719,724,729,734,739,743,
748,753,758,763,768,773,778,783,788,792,797,802,807,812,817,822,
828,833,838,843,848,853,858,863,868,873,879,884,889,894,899,904,
910,915,920,925,931,936,941,947,952,957,962,968,973,978,984,989,
995,1000,1005,1011,1016,1022,1027,1033,1038,1044,1049,1054,1060,1065,1071,1077,
1082,1088,1093,1099,1104,1110,1115,1121,1127,1132,1138,1144,1149,1155,1161,1166,
1172,1178,1183,1189,1195,1200,1206,1212,1218,1223,1229,1235,1241,1246,1252,1258,
1264,1270,1275,1281,1287,1293,1299,1305,1310,1316,1322,1328,1334,1340,1346,1352,
1358,1363,1369,1375,1381,1387,1393,1399,1405,1411,1417,1423,1429,1435,1441,1447,
1453,1459,1465,1471,1477,1483,1489,1495,1501,1507,1513,1519,1526,1532,1538,1544,
1550,1556,1562,1568,1574,1580,1587,1593,1599,1605,1611,1617,1623,1629,1636,1642,
1648,1654,1660,1666,1673,1679,1685,1691,1697,1704,1710,1716,1722,1728,1735,1741,
1747,1753,1759,1766,1772,1778,1784,1791,1797,1803,1809,1816,1822,1828,1834,1841,
1847,1853,1859,1866,1872,1878,1884,1891,1897,1903,1909,1916,1922,1928,1934,1941,
1947,1953,1960,1966,1972,1978,1985,1991,1997,2004,2010,2016,2022,2029,2035,2041};
uint16_t Jag[2000]={
		0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,
		45,48,51,54,57,60,63,66,69,72,75,78,81,84,87,
		90,93,96,99,102,105,108,111,114,117,120,123,126,129,132,
		135,138,141,144,147,150,153,156,159,162,165,168,171,174,177,
		180,183,186,189,192,195,198,201,204,207,210,213,216,219,222,
		225,228,231,234,237,240,243,246,249,252,255,258,261,264,267,
		270,273,276,279,282,285,288,291,294,297,300,303,306,309,312,
		315,318,321,324,327,330,333,336,339,342,345,348,351,354,357,
		360,363,366,369,372,375,378,381,384,387,390,393,396,399,402,
		405,408,411,414,417,420,423,426,429,432,435,438,441,444,447,
		450,453,456,459,462,465,468,471,474,477,480,483,486,489,492,
		495,498,501,504,507,510,513,516,519,522,525,528,531,534,537,
		540,543,546,549,552,555,558,561,564,567,570,573,576,579,582,
		585,588,591,594,597,600,603,606,609,612,615,618,621,624,627,
		630,633,636,639,642,645,648,651,654,657,660,663,666,669,672,
		675,678,681,684,687,690,693,696,699,702,705,708,711,714,717,
		720,723,726,729,732,735,738,741,744,747,750,753,756,759,762,
		765,768,771,774,777,780,783,786,789,792,795,798,801,804,807,
		810,813,816,819,822,825,828,831,834,837,840,843,846,849,852,
		855,858,861,864,867,870,873,876,879,882,885,888,891,894,897,
		900,903,906,909,912,915,918,921,924,927,930,933,936,939,942,
		945,948,951,954,957,960,963,966,969,972,975,978,981,984,987,
		990,993,996,999,1002,1005,1008,1011,1014,1017,1020,1023,1026,1029,1032,
		1035,1038,1041,1044,1047,1050,1053,1056,1059,1062,1065,1068,1071,1074,1077,
		1080,1083,1086,1089,1092,1095,1098,1101,1104,1107,1110,1113,1116,1119,1122,
		1125,1128,1131,1134,1137,1140,1143,1146,1149,1152,1155,1158,1161,1164,1167,
		1170,1173,1176,1179,1182,1185,1188,1191,1194,1197,1200,1203,1206,1209,1212,
		1215,1218,1221,1224,1227,1230,1233,1236,1239,1242,1245,1248,1251,1254,1257,
		1260,1263,1266,1269,1272,1275,1278,1281,1284,1287,1290,1293,1296,1299,1302,
		1305,1308,1311,1314,1317,1320,1323,1326,1329,1332,1335,1338,1341,1344,1347,
		1350,1353,1356,1359,1362,1365,1368,1371,1374,1377,1380,1383,1386,1389,1392,
		1395,1398,1401,1404,1407,1410,1413,1416,1419,1422,1425,1428,1431,1434,1437,
		1440,1443,1446,1449,1452,1455,1458,1461,1464,1467,1470,1473,1476,1479,1482,
		1485,1488,1491,1494,1497,1500,1503,1506,1509,1512,1515,1518,1521,1524,1527,
		1530,1533,1536,1539,1542,1545,1548,1551,1554,1557,1560,1563,1566,1569,1572,
		1575,1578,1581,1584,1587,1590,1593,1596,1599,1602,1605,1608,1611,1614,1617,
		1620,1623,1626,1629,1632,1635,1638,1641,1644,1647,1650,1653,1656,1659,1662,
		1665,1668,1671,1674,1677,1680,1683,1686,1689,1692,1695,1698,1701,1704,1707,
		1710,1713,1716,1719,1722,1725,1728,1731,1734,1737,1740,1743,1746,1749,1752,
		1755,1758,1761,1764,1767,1770,1773,1776,1779,1782,1785,1788,1791,1794,1797,
		1800,1803,1806,1809,1812,1815,1818,1821,1824,1827,1830,1833,1836,1839,1842,
		1845,1848,1851,1854,1857,1860,1863,1866,1869,1872,1875,1878,1881,1884,1887,
		1890,1893,1896,1899,1902,1905,1908,1911,1914,1917,1920,1923,1926,1929,1932,
		1935,1938,1941,1944,1947,1950,1953,1956,1959,1962,1965,1968,1971,1974,1977,
		1980,1983,1986,1989,1992,1995,1998,2001,2004,2007,2010,2013,2016,2019,2022,
		2025,2028,2031,2034,2037,2040,2043,2046,2049,2052,2055,2058,2061,2064,2067,
		2070,2073,2076,2079,2082,2085,2088,2091,2094,2097,2100,2103,2106,2109,2112,
		2115,2118,2121,2124,2127,2130,2133,2136,2139,2142,2145,2148,2151,2154,2157,
		2160,2163,2166,2169,2172,2175,2178,2181,2184,2187,2190,2193,2196,2199,2202,
		2205,2208,2211,2214,2217,2220,2223,2226,2229,2232,2235,2238,2241,2244,2247,
		2250,2253,2256,2259,2262,2265,2268,2271,2274,2277,2280,2283,2286,2289,2292,
		2295,2298,2301,2304,2307,2310,2313,2316,2319,2322,2325,2328,2331,2334,2337,
		2340,2343,2346,2349,2352,2355,2358,2361,2364,2367,2370,2373,2376,2379,2382,
		2385,2388,2391,2394,2397,2400,2403,2406,2409,2412,2415,2418,2421,2424,2427,
		2430,2433,2436,2439,2442,2445,2448,2451,2454,2457,2460,2463,2466,2469,2472,
		2475,2478,2481,2484,2487,2490,2493,2496,2499,2502,2505,2508,2511,2514,2517,
		2520,2523,2526,2529,2532,2535,2538,2541,2544,2547,2550,2553,2556,2559,2562,
		2565,2568,2571,2574,2577,2580,2583,2586,2589,2592,2595,2598,2601,2604,2607,
		2610,2613,2616,2619,2622,2625,2628,2631,2634,2637,2640,2643,2646,2649,2652,
		2655,2658,2661,2664,2667,2670,2673,2676,2679,2682,2685,2688,2691,2694,2697,
		2700,2703,2706,2709,2712,2715,2718,2721,2724,2727,2730,2733,2736,2739,2742,
		2745,2748,2751,2754,2757,2760,2763,2766,2769,2772,2775,2778,2781,2784,2787,
		2790,2793,2796,2799,2802,2805,2808,2811,2814,2817,2820,2823,2826,2829,2832,
		2835,2838,2841,2844,2847,2850,2853,2856,2859,2862,2865,2868,2871,2874,2877,
		2880,2883,2886,2889,2892,2895,2898,2901,2904,2907,2910,2913,2916,2919,2922,
		2925,2928,2931,2934,2937,2940,2943,2946,2949,2952,2955,2958,2961,2964,2967,
		2970,2973,2976,2979,2982,2985,2988,2991,2994,2997,
		3000,2997,2994,2991,2988,2985,2982,2979,2976,2973,2970,2967,2964,2961,2958,
		2955,2952,2949,2946,2943,2940,2937,2934,2931,2928,2925,2922,2919,2916,2913,
		2910,2907,2904,2901,2898,2895,2892,2889,2886,2883,2880,2877,2874,2871,2868,
		2865,2862,2859,2856,2853,2850,2847,2844,2841,2838,2835,2832,2829,2826,2823,
		2820,2817,2814,2811,2808,2805,2802,2799,2796,2793,2790,2787,2784,2781,2778,
		2775,2772,2769,2766,2763,2760,2757,2754,2751,2748,2745,2742,2739,2736,2733,
		2730,2727,2724,2721,2718,2715,2712,2709,2706,2703,2700,2697,2694,2691,2688,
		2685,2682,2679,2676,2673,2670,2667,2664,2661,2658,2655,2652,2649,2646,2643,
		2640,2637,2634,2631,2628,2625,2622,2619,2616,2613,2610,2607,2604,2601,2598,
		2595,2592,2589,2586,2583,2580,2577,2574,2571,2568,2565,2562,2559,2556,2553,
		2550,2547,2544,2541,2538,2535,2532,2529,2526,2523,2520,2517,2514,2511,2508,
		2505,2502,2499,2496,2493,2490,2487,2484,2481,2478,2475,2472,2469,2466,2463,
		2460,2457,2454,2451,2448,2445,2442,2439,2436,2433,2430,2427,2424,2421,2418,
		2415,2412,2409,2406,2403,2400,2397,2394,2391,2388,2385,2382,2379,2376,2373,
		2370,2367,2364,2361,2358,2355,2352,2349,2346,2343,2340,2337,2334,2331,2328,
		2325,2322,2319,2316,2313,2310,2307,2304,2301,2298,2295,2292,2289,2286,2283,
		2280,2277,2274,2271,2268,2265,2262,2259,2256,2253,2250,2247,2244,2241,2238,
		2235,2232,2229,2226,2223,2220,2217,2214,2211,2208,2205,2202,2199,2196,2193,
		2190,2187,2184,2181,2178,2175,2172,2169,2166,2163,2160,2157,2154,2151,2148,
		2145,2142,2139,2136,2133,2130,2127,2124,2121,2118,2115,2112,2109,2106,2103,
		2100,2097,2094,2091,2088,2085,2082,2079,2076,2073,2070,2067,2064,2061,2058,
		2055,2052,2049,2046,2043,2040,2037,2034,2031,2028,2025,2022,2019,2016,2013,
		2010,2007,2004,2001,1998,1995,1992,1989,1986,1983,1980,1977,1974,1971,1968,
		1965,1962,1959,1956,1953,1950,1947,1944,1941,1938,1935,1932,1929,1926,1923,
		1920,1917,1914,1911,1908,1905,1902,1899,1896,1893,1890,1887,1884,1881,1878,
		1875,1872,1869,1866,1863,1860,1857,1854,1851,1848,1845,1842,1839,1836,1833,
		1830,1827,1824,1821,1818,1815,1812,1809,1806,1803,1800,1797,1794,1791,1788,
		1785,1782,1779,1776,1773,1770,1767,1764,1761,1758,1755,1752,1749,1746,1743,
		1740,1737,1734,1731,1728,1725,1722,1719,1716,1713,1710,1707,1704,1701,1698,
		1695,1692,1689,1686,1683,1680,1677,1674,1671,1668,1665,1662,1659,1656,1653,
		1650,1647,1644,1641,1638,1635,1632,1629,1626,1623,1620,1617,1614,1611,1608,
		1605,1602,1599,1596,1593,1590,1587,1584,1581,1578,1575,1572,1569,1566,1563,
		1560,1557,1554,1551,1548,1545,1542,1539,1536,1533,1530,1527,1524,1521,1518,
		1515,1512,1509,1506,1503,1500,1497,1494,1491,1488,1485,1482,1479,1476,1473,
		1470,1467,1464,1461,1458,1455,1452,1449,1446,1443,1440,1437,1434,1431,1428,
		1425,1422,1419,1416,1413,1410,1407,1404,1401,1398,1395,1392,1389,1386,1383,
		1380,1377,1374,1371,1368,1365,1362,1359,1356,1353,1350,1347,1344,1341,1338,
		1335,1332,1329,1326,1323,1320,1317,1314,1311,1308,1305,1302,1299,1296,1293,
		1290,1287,1284,1281,1278,1275,1272,1269,1266,1263,1260,1257,1254,1251,1248,
		1245,1242,1239,1236,1233,1230,1227,1224,1221,1218,1215,1212,1209,1206,1203,
		1200,1197,1194,1191,1188,1185,1182,1179,1176,1173,1170,1167,1164,1161,1158,
		1155,1152,1149,1146,1143,1140,1137,1134,1131,1128,1125,1122,1119,1116,1113,
		1110,1107,1104,1101,1098,1095,1092,1089,1086,1083,1080,1077,1074,1071,1068,
		1065,1062,1059,1056,1053,1050,1047,1044,1041,1038,1035,1032,1029,1026,1023,
		1020,1017,1014,1011,1008,1005,1002,999,996,993,990,987,984,981,978,
		975,972,969,966,963,960,957,954,951,948,945,942,939,936,933,
		930,927,924,921,918,915,912,909,906,903,900,897,894,891,888,
		885,882,879,876,873,870,867,864,861,858,855,852,849,846,843,
		840,837,834,831,828,825,822,819,816,813,810,807,804,801,798,
		795,792,789,786,783,780,777,774,771,768,765,762,759,756,753,
		750,747,744,741,738,735,732,729,726,723,720,717,714,711,708,
		705,702,699,696,693,690,687,684,681,678,675,672,669,666,663,
		660,657,654,651,648,645,642,639,636,633,630,627,624,621,618,
		615,612,609,606,603,600,597,594,591,588,585,582,579,576,573,
		570,567,564,561,558,555,552,549,546,543,540,537,534,531,528,
		525,522,519,516,513,510,507,504,501,498,495,492,489,486,483,
		480,477,474,471,468,465,462,459,456,453,450,447,444,441,438,
		435,432,429,426,423,420,417,414,411,408,405,402,399,396,393,
		390,387,384,381,378,375,372,369,366,363,360,357,354,351,348,
		345,342,339,336,333,330,327,324,321,318,315,312,309,306,303,
		300,297,294,291,288,285,282,279,276,273,270,267,264,261,258,
		255,252,249,246,243,240,237,234,231,228,225,222,219,216,213,
		210,207,204,201,198,195,192,189,186,183,180,177,174,171,168,
		165,162,159,156,153,150,147,144,141,138,135,132,129,126,123,
		120,117,114,111,108,105,102,99,96,93,90,87,84,81,78,
		75,72,69,66,63,60,57,54,51,48,45,42,39,36,33,
		30,27,24,21,18,15,12,9,6,3};
uint16_t dmaRxBuffer[_Size];
volatile uint32_t RxLength = 2048;
static int convertFlag;

/**
 * @brief DAC使能函数
 * @retval None
 */
void DAC_Init(){
	HAL_TIM_Base_Start(&htim6);
	HAL_TIM_Base_Start(&htim5);
	HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)dmaRxBuffer,RxLength,DAC_ALIGN_12B_R);
}

/**
 * @brief DMA流处理函数
 * @retval None
 */
void DMA1_Stream_Handler(){

	float32_t sinConvert0[_Size/2];
	float32_t sinConvert1[_Size/2];
	float32_t magChgPort0[_Size/2];
	float32_t magChgPort1[_Size/2];

	/*判断是否接收完成*/
	if(__HAL_DMA_GET_FLAG(&hdma_dac1,DMA_FLAG_TCIF1_5) != RESET){
		__HAL_DMA_CLEAR_FLAG(&hdma_dac1,DMA_FLAG_TCIF1_5);		//清除DMA接收完成标志

		/*以下为对每次传输完成后，对为输出的后半部分存储区进行数据处理*/
		//下列为修改dmaBuffer数组后半部步进值并修改幅值
		for(int i = 1024;i < 2048;i++){
			sinConvert1[i-1024] = (float32_t)sinWave[step*i%2048];		//修改步进值
			magChgPort1[i-1024] = (float32_t)(amplitude)/100;
		}
		arm_sub_f32(sinConvert1,offset,sinConvert1,1024);				//去除波形直流偏置
		arm_mult_f32(sinConvert1,magChgPort1,sinConvert1,1024);			//幅值变化
		arm_add_f32(sinConvert1,offset,sinConvert1,1024);				//恢复直流偏置
		for(int i = 1024;i < 2048;i++){
			dmaRxBuffer[i] = sinConvert1[i-1024];
		}
	}

	/*判断是否接收半完成*/
	if(__HAL_DMA_GET_FLAG(&hdma_dac1,DMA_FLAG_HTIF1_5) != RESET){
		__HAL_DMA_CLEAR_FLAG(&hdma_dac1,DMA_FLAG_HTIF1_5);

		/*以下为对每次半传输完成后，对为输出的前半部分存储区进行数据处理*/
		//下列为修改dmaBuffer数组前半部步进值并修改幅值
		for(int i = 0;i < 1024;i++){
			sinConvert0[i] = (float32_t)sinWave[step*i%2048];			//修改步进值
			magChgPort0[i] = (float32_t)(amplitude)/100;
		}
		arm_sub_f32(sinConvert0,offset,sinConvert0,1024);				//去除波形直流偏置
		arm_mult_f32(sinConvert0,magChgPort0,sinConvert0,1024);			//幅值变化
		arm_add_f32(sinConvert0,offset,sinConvert0,1024);				//恢复直流偏置
		for(int i = 0;i < 1024;i++){
			dmaRxBuffer[i] = sinConvert0[i];
		}
	}
}
/**
 * @brief DMA流处理函数
 * @retval None
 */
void DMA1_Stream5_IRQHandler(void){
	DMA1_Stream_Handler();
}

/**
 * @brief 扫频模式启动函数
 * @retval None
 */
void sweepFrequencyOn(){
	step = 1;
	convertFlag = 0;
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_2,(uint32_t*)Jag,1000,DAC_ALIGN_12B_R);
}

/**
 * @brief 扫频模式关闭函数
 * @retval None
 */
void sweepFrequencyOff(){
	HAL_TIM_Base_Stop(&htim7);
	HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_2);
}

/**
 * @brief 定时器中断回调函数
 * @retval None
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim7.Instance) //判断是否定时器
	{
		if(convertFlag == 0){
			if(step== 40){
				convertFlag = 1;
				step--;
			}
			step++;
		}
		else if(convertFlag == 1){
			if(step == 1){
				convertFlag = 0;
				step++;
			}
			step--;
		}
	}
}
