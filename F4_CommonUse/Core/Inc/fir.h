/*
 * fir.h
 *
 *  Created on: 2022年7月23日
 *      Author: 王志涵
 */

#ifndef INC_FIR_H_
#define INC_FIR_H_

#include "base.h"
//
//const float LowPass64[65]={
//  0.0003044810146,-0.000589467003,0.0008616039413,-0.001094589126, 0.001230889931,
//  -0.001186058624,0.0008669441449,-0.0002014238562,-0.0008265857468, 0.002145854523,
//   -0.00358020328, 0.004852957558,-0.005618192721, 0.005518035963,-0.004259138368,
//   0.001695920364, 0.002095511416,-0.006769643631,  0.01170436013, -0.01604282297,
//    0.01878481731, -0.01891879551,  0.01557734236,-0.008192637935,-0.003373990068,
//     0.0187523365, -0.03705101088,  0.05692031607, -0.07668624818,   0.0945398286,
//    -0.1087563634,   0.1179132387,   0.8787654638,   0.1179132387,  -0.1087563634,
//     0.0945398286, -0.07668624818,  0.05692031607, -0.03705101088,   0.0187523365,
//  -0.003373990068,-0.008192637935,  0.01557734236, -0.01891879551,  0.01878481731,
//   -0.01604282297,  0.01170436013,-0.006769643631, 0.002095511416, 0.001695920364,
//  -0.004259138368, 0.005518035963,-0.005618192721, 0.004852957558, -0.00358020328,
//   0.002145854523,-0.0008265857468,-0.0002014238562,0.0008669441449,-0.001186058624,
//   0.001230889931,-0.001094589126,0.0008616039413,-0.000589467003,0.0003044810146
//};
//
//const float LowPass128[129]={
//    0.05154783651, 0.003607540624, 0.003731560893, 0.003856387921, 0.003981932998,
//    0.00410810532, 0.004234813154, 0.004361961968,   0.0044894577, 0.004617203027,
//   0.004745101091, 0.004873052705, 0.005000957754, 0.005128716119, 0.005256226286,
//   0.005383385345, 0.005510091782, 0.005636241287,  0.00576173095, 0.005886457395,
//   0.006010315847, 0.006133203395, 0.006255015731,  0.00637565041, 0.006495003123,
//   0.006612972822, 0.006729456596, 0.006844353862, 0.006957564037, 0.007068988401,
//   0.007178528234, 0.007286087144, 0.007391568739, 0.007494879887,  0.00759592792,
//    0.00769462157,  0.00779087143, 0.007884590887, 0.007975695655, 0.008064100519,
//   0.008149726316, 0.008232494816, 0.008312327787, 0.008389154449, 0.008462901227,
//   0.008533501998, 0.008600889705, 0.008665001951, 0.008725779131, 0.008783165365,
//   0.008837105706, 0.008887549862, 0.008934450336, 0.008977763355, 0.009017448872,
//   0.009053468704, 0.009085788392, 0.009114379063, 0.009139211848, 0.009160264395,
//   0.009177516215, 0.009190951474, 0.009200557135, 0.009206323884, 0.009208247066,
//   0.009206323884, 0.009200557135, 0.009190951474, 0.009177516215, 0.009160264395,
//   0.009139211848, 0.009114379063, 0.009085788392, 0.009053468704, 0.009017448872,
//   0.008977763355, 0.008934450336, 0.008887549862, 0.008837105706, 0.008783165365,
//   0.008725779131, 0.008665001951, 0.008600889705, 0.008533501998, 0.008462901227,
//   0.008389154449, 0.008312327787, 0.008232494816, 0.008149726316, 0.008064100519,
//   0.007975695655, 0.007884590887,  0.00779087143,  0.00769462157,  0.00759592792,
//   0.007494879887, 0.007391568739, 0.007286087144, 0.007178528234, 0.007068988401,
//   0.006957564037, 0.006844353862, 0.006729456596, 0.006612972822, 0.006495003123,
//    0.00637565041, 0.006255015731, 0.006133203395, 0.006010315847, 0.005886457395,
//    0.00576173095, 0.005636241287, 0.005510091782, 0.005383385345, 0.005256226286,
//   0.005128716119, 0.005000957754, 0.004873052705, 0.004745101091, 0.004617203027,
//     0.0044894577, 0.004361961968, 0.004234813154,  0.00410810532, 0.003981932998,
//   0.003856387921, 0.003731560893, 0.003607540624,  0.05154783651
//};



//低通布莱克曼滤波器，阶数64阶，采样频率200Hz，步进10Hz，从10Hz~90Hz（过渡带+-5Hz左右）

//const float LowPassFIR[9][65] = {
//                -0,-2.762588792e-06,1.371575154e-20,2.730154483e-05,9.777231026e-05,
//  0.0002240430185,0.0004062141234,0.0006253258325,0.0008383301902, 0.000976504467,
//  0.0009493103717,0.0006552264676,-6.709853383e-19,-0.001078819623,-0.002584029688,
//   -0.00443225028,-0.006434465293,-0.008289490826,-0.009595135227,-0.009879508987,
//  -0.008651672862,-0.005467108916,2.601326483e-18, 0.007888187654,  0.01809531823,
//    0.03025088273,  0.04372153431,  0.05765283853,  0.07104469836,  0.08285246044,
//    0.09210107476,  0.09799704701,   0.1000223607,  0.09799704701,  0.09210107476,
//    0.08285246044,  0.07104469836,  0.05765283853,  0.04372153431,  0.03025088273,
//    0.01809531823, 0.007888187654,2.601326483e-18,-0.005467108916,-0.008651672862,
//  -0.009879508987,-0.009595135227,-0.008289490826,-0.006434465293, -0.00443225028,
//  -0.002584029688,-0.001078819623,-6.709853383e-19,0.0006552264676,0.0009493103717,
//   0.000976504467,0.0008383301902,0.0006253258325,0.0004062141234,0.0002240430185,
//  9.777231026e-05,2.730154483e-05,1.371575154e-20,-2.762588792e-06,             -0,
//
//
//                  0,5.253309155e-06,-2.742394859e-20,-5.191632226e-05,-0.0001581553515,
//  -0.0002633058175,-0.0002509850019,3.827962074e-19,0.0005179739092, 0.001147633768,
//   0.001535593416, 0.001245971536,-1.341601134e-18,-0.002051471733,-0.004179896321,
//  -0.005208987743,-0.003975623287,3.044670889e-18, 0.005928486586,  0.01161086094,
//    0.01399484649,  0.01039619558,-5.201220288e-18, -0.01500009187, -0.02927077562,
//   -0.03555225208, -0.02701395378,7.058491813e-18,  0.04389594495,  0.09737208486,
//     0.1489816308,   0.1863501221,   0.1999896318,   0.1863501221,   0.1489816308,
//    0.09737208486,  0.04389594495,7.058491813e-18, -0.02701395378, -0.03555225208,
//   -0.02927077562, -0.01500009187,-5.201220288e-18,  0.01039619558,  0.01399484649,
//    0.01161086094, 0.005928486586,3.044670889e-18,-0.003975623287,-0.005208987743,
//  -0.004179896321,-0.002051471733,-1.341601134e-18, 0.001245971536, 0.001535593416,
//   0.001147633768,0.0005179739092,3.827962074e-19,-0.0002509850019,-0.0002633058175,
//  -0.0001581553515,-5.191632226e-05,-2.742394859e-20,5.253309155e-06,              0,
//
//
//
//                  -0,-7.230873962e-06,4.113771458e-20,7.145979907e-05,  0.00015816222,
//  8.555696695e-05,-0.0002509959158,-0.0006251808372,-0.0005179963773,0.0003729050222,
//   0.001535660238, 0.001715007238,-2.012489278e-18,-0.002823731396,-0.004180078395,
//  -0.001692576334, 0.003975796048, 0.008287568577, 0.005928744562, -0.00377276144,
//   -0.01399545465, -0.01430975739, 7.80216937e-18,  0.02064675279,   0.0292720478,
//    0.01155212894, -0.02701512724, -0.05763946846,  -0.0438978523,  0.03163948283,
//     0.1489881128,   0.2565000951,   0.2999974787,   0.2565000951,   0.1489881128,
//    0.03163948283,  -0.0438978523, -0.05763946846, -0.02701512724,  0.01155212894,
//     0.0292720478,  0.02064675279, 7.80216937e-18, -0.01430975739, -0.01399545465,
//   -0.00377276144, 0.005928744562, 0.008287568577, 0.003975796048,-0.001692576334,
//  -0.004180078395,-0.002823731396,-2.012489278e-18, 0.001715007238, 0.001535660238,
//  0.0003729050222,-0.0005179963773,-0.0006251808372,-0.0002509959158,8.555696695e-05,
//    0.00015816222,7.145979907e-05,4.113771458e-20,-7.230873962e-06,             -0,
//
//
//	                0,8.500581316e-06,-5.485144502e-20,-8.400779916e-05,-9.775169747e-05,
//  0.0001627424645,0.0004061284999,-7.656418388e-19,-0.0008381534717,-0.0007093224558,
//  0.0009491102537, 0.002016154584,-2.683375561e-18, -0.00331956544,-0.002583484864,
//   0.003219539532, 0.006433108822,-6.089735102e-18,-0.009593112394,-0.007176369894,
//   0.008649849333,  0.01682248339,-1.040311225e-17,  -0.0242722258, -0.01809150167,
//    0.02197391912,  0.04371231794,-1.411789518e-17, -0.07102972269, -0.06018314511,
//    0.09208165854,   0.3015403152,   0.4000050724,   0.3015403152,  0.09208165854,
//   -0.06018314511, -0.07102972269,-1.411789518e-17,  0.04371231794,  0.02197391912,
//   -0.01809150167,  -0.0242722258,-1.040311225e-17,  0.01682248339, 0.008649849333,
//  -0.007176369894,-0.009593112394,-6.089735102e-18, 0.006433108822, 0.003219539532,
//  -0.002583484864, -0.00331956544,-2.683375561e-18, 0.002016154584,0.0009491102537,
//  -0.0007093224558,-0.0008381534717,-7.656418388e-19,0.0004061284999,0.0001627424645,
//  -9.775169747e-05,-8.400779916e-05,-5.485144502e-20,8.500581316e-06,              0,
//
//
//                 -0,-8.937851817e-06,2.011639143e-19,8.832916501e-05,-2.851250212e-19,
//  -0.0002768681734,-8.372460047e-19, 0.000625180779,-1.295090629e-18,-0.001206745976,
//  7.911692219e-18, 0.002119865501,-3.354148521e-18,-0.003490323666,4.844304675e-18,
//   0.005477291998,-6.626836296e-18,-0.008287568577,8.646750554e-18,  0.01220891159,
//  -1.081291489e-17, -0.01768783107,1.300361506e-17,  0.02552079037,-1.507709382e-17,
//   -0.03738347441,  1.6885754e-17,  0.05763946474,-1.829217942e-17,  -0.1023875177,
//  1.918474866e-17,   0.3170515299,   0.4999957681,   0.3170515299,1.918474866e-17,
//    -0.1023875177,-1.829217942e-17,  0.05763946474,  1.6885754e-17, -0.03738347441,
//  -1.507709382e-17,  0.02552079037,1.300361506e-17, -0.01768783107,-1.081291489e-17,
//    0.01220891159,8.646750554e-18,-0.008287568577,-6.626836296e-18, 0.005477291998,
//  4.844304675e-18,-0.003490323666,-3.354148521e-18, 0.002119865501,7.911692219e-18,
//  -0.001206745976,-1.295090629e-18, 0.000625180779,-8.372460047e-19,-0.0002768681734,
//  -2.851250212e-19,8.832916501e-05,2.011639143e-19,-8.937851817e-06,             -0,
//
//
//                 -0,8.500507647e-06,-8.227644374e-20,-8.400706429e-05, 9.77508389e-05,
//  0.0001627410384,-0.0004061249492,3.369568615e-18,0.0008381461375,-0.0007093162858,
//   -0.00094910193, 0.002016136888,-4.025028187e-18,-0.003319536336, 0.002583462512,
//    0.00321951136,-0.006433052476,9.134522417e-18, 0.009593028575,-0.007176307496,
//  -0.008649773896,  0.01682233624,-1.56045323e-17, -0.02427201346,  0.01809134521,
//    0.02197372727, -0.04371193796,2.117665913e-17,  0.07102909684, -0.06018261611,
//   -0.09208085388,   0.3015376627,    0.600002408,   0.3015376627, -0.09208085388,
//   -0.06018261611,  0.07102909684,2.117665913e-17, -0.04371193796,  0.02197372727,
//    0.01809134521, -0.02427201346,-1.56045323e-17,  0.01682233624,-0.008649773896,
//  -0.007176307496, 0.009593028575,9.134522417e-18,-0.006433052476,  0.00321951136,
//   0.002583462512,-0.003319536336,-4.025028187e-18, 0.002016136888, -0.00094910193,
//  -0.0007093162858,0.0008381461375,3.369568615e-18,-0.0004061249492,0.0001627410384,
//   9.77508389e-05,-8.400706429e-05,-8.227644374e-20,8.500507647e-06,             -0,
//
//
//                   0,-7.230927622e-06,-3.661333389e-20,7.146032294e-05,-0.0001581633987,
//  8.555760724e-05,0.0002509977785,-0.0006251854356, 0.000518000219,0.0003729077871,
//  -0.001535671647, 0.001715020044,-4.695843126e-18,-0.002823752351, 0.004180109128,
//  -0.001692588907,-0.003975825384, 0.008287630044,-0.005928788334, -0.00377278938,
//    0.01399555802, -0.01430986356,1.820519642e-17,  0.02064690553, -0.02927226573,
//    0.01155221462,  0.02701532841, -0.05763989687,  0.04389818013,  0.03163971752,
//    -0.1489892155,   0.2565020025,   0.6999993324,   0.2565020025,  -0.1489892155,
//    0.03163971752,  0.04389818013, -0.05763989687,  0.02701532841,  0.01155221462,
//   -0.02927226573,  0.02064690553,1.820519642e-17, -0.01430986356,  0.01399555802,
//   -0.00377278938,-0.005928788334, 0.008287630044,-0.003975825384,-0.001692588907,
//   0.004180109128,-0.002823752351,-4.695843126e-18, 0.001715020044,-0.001535671647,
//  0.0003729077871, 0.000518000219,-0.0006251854356,0.0002509977785,8.555760724e-05,
//  -0.0001581633987,7.146032294e-05,-3.661333389e-20,-7.230927622e-06,              0,
//
//                 -0,5.253579729e-06,-1.097014554e-19,-5.191899618e-05,0.0001581635006,
//  -0.0002633193799, 0.000250997924,-1.531263618e-18,-0.0005180005683,  0.00114769279,
//  -0.001535672578, 0.001246035681,-5.366680813e-18,-0.002051577438, 0.004180111922,
//  -0.005209255964, 0.003975827713,-1.217931056e-17,-0.005928792059,  0.01161145885,
//   -0.01399556641,  0.01039673015,-2.080595152e-17, -0.01500086486,   0.0292722825,
//    -0.0355540812,  0.02701534517,-2.823542309e-17,  -0.0438982062,   0.0973770991,
//    -0.1489893049,   0.1863597184,   0.7999997139,   0.1863597184,  -0.1489893049,
//     0.0973770991,  -0.0438982062,-2.823542309e-17,  0.02701534517,  -0.0355540812,
//     0.0292722825, -0.01500086486,-2.080595152e-17,  0.01039673015, -0.01399556641,
//    0.01161145885,-0.005928792059,-1.217931056e-17, 0.003975827713,-0.005209255964,
//   0.004180111922,-0.002051577438,-5.366680813e-18, 0.001246035681,-0.001535672578,
//    0.00114769279,-0.0005180005683,-1.531263618e-18, 0.000250997924,-0.0002633193799,
//  0.0001581635006,-5.191899618e-05,-1.097014554e-19,5.253579729e-06,             -0,
//
//
//                  0,-2.761972837e-06,2.560164838e-19,2.729545668e-05,-9.775050421e-05,
//  0.0002239930473,-0.0004061235522, 0.000625186367,-0.0008381432854,0.0009762867121,
//  -0.0009490986704,0.0006550803664,-6.037521394e-18,-0.001078578993, 0.002583453432,
//  -0.004431261681,  0.00643303059,-0.008287642151, 0.009592995979,-0.009877305478,
//   0.008649744093,-0.005465889815,2.340671656e-17, 0.007886428386, -0.01809128188,
//    0.03024413623, -0.04371178523,  0.05763998255, -0.07102885097,    0.082833983,
//   -0.09208054096,    0.097975187,    0.900000453,    0.097975187, -0.09208054096,
//      0.082833983, -0.07102885097,  0.05763998255, -0.04371178523,  0.03024413623,
//   -0.01809128188, 0.007886428386,2.340671656e-17,-0.005465889815, 0.008649744093,
//  -0.009877305478, 0.009592995979,-0.008287642151,  0.00643303059,-0.004431261681,
//   0.002583453432,-0.001078578993,-6.037521394e-18,0.0006550803664,-0.0009490986704,
//  0.0009762867121,-0.0008381432854, 0.000625186367,-0.0004061235522,0.0002239930473,
//  -9.775050421e-05,2.729545668e-05,2.560164838e-19,-2.761972837e-06,              0
//
//
//};

#endif /* INC_FIR_H_ */
