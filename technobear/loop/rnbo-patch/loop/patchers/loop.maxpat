{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 5,
			"revision" : 1,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 635.0, 228.0, 1968.0, 1151.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"fontsize" : 48.0,
					"id" : "obj-55",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 664.0, 2.0, 670.0, 60.0 ],
					"text" : "LOOP MODULE FOR SSP ",
					"textcolor" : [ 0.890196078431372, 0.031372549019608, 0.031372549019608, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-48",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 335.25, 94.0, 29.5, 22.0 ],
					"text" : "3"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-47",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "signal" ],
					"patching_rect" : [ 381.0, 94.0, 35.0, 22.0 ],
					"text" : "adc~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-77",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 1550.0, 439.0, 43.0, 22.0 ],
					"text" : "* 1000"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-78",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1550.0, 471.5, 63.0, 22.0 ],
					"text" : "setsize $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-75",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 1307.5, 439.0, 43.0, 22.0 ],
					"text" : "* 1000"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-76",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1307.5, 471.5, 63.0, 22.0 ],
					"text" : "setsize $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-73",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 1550.0, 592.0, 43.0, 22.0 ],
					"text" : "* 1000"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-74",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1550.0, 624.5, 63.0, 22.0 ],
					"text" : "setsize $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-72",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 1303.0, 593.0, 43.0, 22.0 ],
					"text" : "* 1000"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-69",
					"maxclass" : "number",
					"maximum" : 180,
					"minimum" : 1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1303.0, 520.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer3/size",
					"id" : "obj-70",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1343.5, 558.5, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-67",
					"maxclass" : "number",
					"maximum" : 180,
					"minimum" : 1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1550.0, 513.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer4/size",
					"id" : "obj-68",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1576.0, 550.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-63",
					"maxclass" : "number",
					"maximum" : 180,
					"minimum" : 1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1550.0, 374.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer2/size",
					"id" : "obj-64",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1576.0, 409.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-57",
					"maxclass" : "number",
					"maximum" : 180,
					"minimum" : 1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1307.5, 370.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-50",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1192.5, 416.0, 35.0, 22.0 ],
					"text" : "clear"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-54",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1507.0, 224.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-52",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 1428.0, 322.0, 29.5, 22.0 ],
					"text" : "*~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-51",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 1428.0, 286.0, 53.0, 22.0 ],
					"text" : "cycle~ 1"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-41",
					"maxclass" : "number~",
					"mode" : 1,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 1182.0, 286.0, 56.0, 22.0 ],
					"sig" : 0.6
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-40",
					"maxclass" : "number~",
					"mode" : 1,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 953.5, 286.0, 56.0, 22.0 ],
					"sig" : 0.4
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-43",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 288.0, 74.0, 29.5, 22.0 ],
					"text" : "0"
				}

			}
, 			{
				"box" : 				{
					"buffername" : "layer1",
					"id" : "obj-42",
					"maxclass" : "waveform~",
					"numinlets" : 5,
					"numoutlets" : 6,
					"outlettype" : [ "float", "float", "float", "float", "list", "" ],
					"patching_rect" : [ 335.25, 833.0, 524.0, 176.0 ]
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-34",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 944.0, 706.0, 163.0, 77.0 ],
					"range" : [ 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-33",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 755.0, 706.0, 163.0, 77.0 ],
					"range" : [ 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-32",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 566.0, 706.0, 163.0, 77.0 ],
					"range" : [ 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-30",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 377.0, 706.0, 163.0, 77.0 ],
					"range" : [ 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-29",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 198.0, 706.0, 163.0, 77.0 ],
					"range" : [ 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-38",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 335.25, 141.0, 29.5, 22.0 ],
					"text" : "2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 273.0, 141.0, 29.5, 22.0 ],
					"text" : "1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 4,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 319.0, 235.0, 68.0, 22.0 ],
					"text" : "selector~ 3"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 191.5, 1025.0, 35.0, 22.0 ],
					"text" : "dac~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
					"maxclass" : "gain~",
					"multichannelvariant" : 0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 198.0, 841.0, 22.0, 140.0 ]
				}

			}
, 			{
				"box" : 				{
					"autosave" : 1,
					"id" : "obj-17",
					"inletInfo" : 					{
						"IOInfo" : [ 							{
								"type" : "signal",
								"index" : 1,
								"tag" : "in1",
								"comment" : ""
							}
, 							{
								"type" : "signal",
								"index" : 2,
								"tag" : "in2",
								"comment" : "layer1/rate"
							}
, 							{
								"type" : "signal",
								"index" : 3,
								"tag" : "in3",
								"comment" : "layer1/begin"
							}
, 							{
								"type" : "signal",
								"index" : 4,
								"tag" : "in4",
								"comment" : "layer1/end"
							}
, 							{
								"type" : "signal",
								"index" : 5,
								"tag" : "in5",
								"comment" : "layer2/rate"
							}
, 							{
								"type" : "signal",
								"index" : 6,
								"tag" : "in6",
								"comment" : "layer2/begin"
							}
, 							{
								"type" : "signal",
								"index" : 7,
								"tag" : "in7",
								"comment" : "layer2/end"
							}
, 							{
								"type" : "signal",
								"index" : 8,
								"tag" : "in8",
								"comment" : "layer3/rate"
							}
, 							{
								"type" : "signal",
								"index" : 9,
								"tag" : "in9",
								"comment" : "layer3/begin"
							}
, 							{
								"type" : "signal",
								"index" : 10,
								"tag" : "in10",
								"comment" : "layer3/end"
							}
, 							{
								"type" : "signal",
								"index" : 11,
								"tag" : "in11",
								"comment" : "layer4/rate"
							}
, 							{
								"type" : "signal",
								"index" : 12,
								"tag" : "in12",
								"comment" : "layer4/begin"
							}
, 							{
								"type" : "signal",
								"index" : 13,
								"tag" : "in13",
								"comment" : "layer4/end"
							}
, 							{
								"type" : "signal",
								"index" : 14,
								"tag" : "in14",
								"comment" : "rec-mode"
							}
, 							{
								"type" : "signal",
								"index" : 15,
								"tag" : "in15",
								"comment" : "rec-begin"
							}
, 							{
								"type" : "signal",
								"index" : 16,
								"tag" : "in16",
								"comment" : "rec-end"
							}
 ]
					}
,
					"maxclass" : "newobj",
					"numinlets" : 16,
					"numoutlets" : 11,
					"outletInfo" : 					{
						"IOInfo" : [ 							{
								"type" : "signal",
								"index" : 1,
								"tag" : "out1",
								"comment" : "sum"
							}
, 							{
								"type" : "signal",
								"index" : 2,
								"tag" : "out2",
								"comment" : "layer1"
							}
, 							{
								"type" : "signal",
								"index" : 3,
								"tag" : "out3",
								"comment" : "layer2"
							}
, 							{
								"type" : "signal",
								"index" : 4,
								"tag" : "out4",
								"comment" : "layer3"
							}
, 							{
								"type" : "signal",
								"index" : 5,
								"tag" : "out5",
								"comment" : "layer4"
							}
, 							{
								"type" : "signal",
								"index" : 6,
								"tag" : "out6",
								"comment" : "record-pos"
							}
, 							{
								"type" : "signal",
								"index" : 7,
								"tag" : "out7",
								"comment" : "layer1-pos"
							}
, 							{
								"type" : "signal",
								"index" : 8,
								"tag" : "out8",
								"comment" : "layer2-pos"
							}
, 							{
								"type" : "signal",
								"index" : 9,
								"tag" : "out9",
								"comment" : "layer3-pos"
							}
, 							{
								"type" : "signal",
								"index" : 10,
								"tag" : "out10",
								"comment" : "layer4-pos"
							}
 ]
					}
,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "list" ],
					"patching_rect" : [ 319.0, 365.0, 213.0, 22.0 ],
					"rnboattrcache" : 					{
						"layer3/gain" : 						{
							"label" : "gain",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer2/gain" : 						{
							"label" : "gain",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/xfade" : 						{
							"label" : "xfade",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer3/end" : 						{
							"label" : "end",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/end" : 						{
							"label" : "end",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"rec-begin" : 						{
							"label" : "rec-begin",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/size" : 						{
							"label" : "size",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/rate" : 						{
							"label" : "rate",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"rec-layer" : 						{
							"label" : "rec-layer",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer2/begin" : 						{
							"label" : "begin",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/rate" : 						{
							"label" : "rate",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/xfade" : 						{
							"label" : "xfade",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer2/rate" : 						{
							"label" : "rate",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"rec-mon" : 						{
							"label" : "rec-mon",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/gain" : 						{
							"label" : "gain",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer2/end" : 						{
							"label" : "end",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer3/xfade" : 						{
							"label" : "xfade",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer3/size" : 						{
							"label" : "size",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"rec-gain" : 						{
							"label" : "rec-gain",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/size" : 						{
							"label" : "size",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer2/xfade" : 						{
							"label" : "xfade",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer2/size" : 						{
							"label" : "size",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/begin" : 						{
							"label" : "begin",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/begin" : 						{
							"label" : "begin",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer3/begin" : 						{
							"label" : "begin",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/gain" : 						{
							"label" : "gain",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/end" : 						{
							"label" : "end",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"rec-end" : 						{
							"label" : "rec-end",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer3/rate" : 						{
							"label" : "rate",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"rec-mode" : 						{
							"label" : "rec-mode",
							"isEnum" : 1,
							"parsestring" : "Stop Record"
						}
,
						"rec-loop" : 						{
							"label" : "rec-loop",
							"isEnum" : 1,
							"parsestring" : "false true"
						}
,
						"layer1/mode" : 						{
							"label" : "mode",
							"isEnum" : 1,
							"parsestring" : "Stop Play"
						}
,
						"layer1/loop" : 						{
							"label" : "loop",
							"isEnum" : 1,
							"parsestring" : "false true"
						}
,
						"layer2/mode" : 						{
							"label" : "mode",
							"isEnum" : 1,
							"parsestring" : "Stop Play"
						}
,
						"layer2/loop" : 						{
							"label" : "loop",
							"isEnum" : 1,
							"parsestring" : "false true"
						}
,
						"layer3/mode" : 						{
							"label" : "mode",
							"isEnum" : 1,
							"parsestring" : "Stop Play"
						}
,
						"layer3/loop" : 						{
							"label" : "loop",
							"isEnum" : 1,
							"parsestring" : "false true"
						}
,
						"layer4/mode" : 						{
							"label" : "mode",
							"isEnum" : 1,
							"parsestring" : "Stop Play"
						}
,
						"layer4/loop" : 						{
							"label" : "loop",
							"isEnum" : 1,
							"parsestring" : "false true"
						}

					}
,
					"rnboversion" : "1.0.1",
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_invisible" : 1,
							"parameter_longname" : "rnbo~",
							"parameter_shortname" : "rnbo~",
							"parameter_type" : 3
						}

					}
,
					"saved_object_attributes" : 					{
						"optimization" : "O1",
						"parameter_enable" : 1,
						"uuid" : "82e7dce7-635c-11ed-9ce7-6e9de1f20474"
					}
,
					"snapshot" : 					{
						"filetype" : "C74Snapshot",
						"version" : 2,
						"minorversion" : 0,
						"name" : "snapshotlist",
						"origin" : "rnbo~",
						"type" : "list",
						"subtype" : "Undefined",
						"embed" : 1,
						"snapshot" : 						{
							"__sps" : 							{
								"layer4" : 								{
									"xfade" : 									{
										"value" : 1.0
									}
,
									"gain" : 									{
										"value" : 1.0
									}
,
									"loop" : 									{
										"value" : 1.0
									}
,
									"begin" : 									{
										"value" : 0.0
									}
,
									"size" : 									{
										"value" : 10.0
									}
,
									"mode" : 									{
										"value" : 0.0
									}
,
									"end" : 									{
										"value" : 1.0
									}
,
									"rate" : 									{
										"value" : 1.0
									}

								}
,
								"layer3" : 								{
									"xfade" : 									{
										"value" : 1.0
									}
,
									"gain" : 									{
										"value" : 1.0
									}
,
									"loop" : 									{
										"value" : 1.0
									}
,
									"begin" : 									{
										"value" : 0.0
									}
,
									"size" : 									{
										"value" : 2.0
									}
,
									"mode" : 									{
										"value" : 1.0
									}
,
									"end" : 									{
										"value" : 1.0
									}
,
									"rate" : 									{
										"value" : 1.0
									}

								}
,
								"layer2" : 								{
									"xfade" : 									{
										"value" : 1.0
									}
,
									"gain" : 									{
										"value" : 1.0
									}
,
									"loop" : 									{
										"value" : 1.0
									}
,
									"begin" : 									{
										"value" : 0.0
									}
,
									"size" : 									{
										"value" : 10.0
									}
,
									"mode" : 									{
										"value" : 0.0
									}
,
									"end" : 									{
										"value" : 1.0
									}
,
									"rate" : 									{
										"value" : 1.0
									}

								}
,
								"layer1" : 								{
									"xfade" : 									{
										"value" : 1.0
									}
,
									"gain" : 									{
										"value" : 1.0
									}
,
									"loop" : 									{
										"value" : 1.0
									}
,
									"begin" : 									{
										"value" : 0.0
									}
,
									"size" : 									{
										"value" : 3.0
									}
,
									"mode" : 									{
										"value" : 1.0
									}
,
									"end" : 									{
										"value" : 1.0
									}
,
									"rate" : 									{
										"value" : 1.0
									}

								}

							}
,
							"rec-begin" : 							{
								"value" : 0.0
							}
,
							"rec-mode" : 							{
								"value" : 0.0
							}
,
							"rec-loop" : 							{
								"value" : 1.0
							}
,
							"rec-gain" : 							{
								"value" : 1.0
							}
,
							"rec-mon" : 							{
								"value" : 100.0
							}
,
							"rec-end" : 							{
								"value" : 0.0
							}
,
							"rec-layer" : 							{
								"value" : 0.0
							}
,
							"__presetid" : "loop"
						}
,
						"snapshotlist" : 						{
							"current_snapshot" : 0,
							"entries" : [ 								{
									"filetype" : "C74Snapshot",
									"version" : 2,
									"minorversion" : 0,
									"name" : "loop",
									"origin" : "loop",
									"type" : "rnbo",
									"subtype" : "",
									"embed" : 0,
									"snapshot" : 									{
										"__sps" : 										{
											"layer4" : 											{
												"xfade" : 												{
													"value" : 1.0
												}
,
												"gain" : 												{
													"value" : 1.0
												}
,
												"loop" : 												{
													"value" : 1.0
												}
,
												"begin" : 												{
													"value" : 0.0
												}
,
												"size" : 												{
													"value" : 10.0
												}
,
												"mode" : 												{
													"value" : 0.0
												}
,
												"end" : 												{
													"value" : 1.0
												}
,
												"rate" : 												{
													"value" : 1.0
												}

											}
,
											"layer3" : 											{
												"xfade" : 												{
													"value" : 1.0
												}
,
												"gain" : 												{
													"value" : 1.0
												}
,
												"loop" : 												{
													"value" : 1.0
												}
,
												"begin" : 												{
													"value" : 0.0
												}
,
												"size" : 												{
													"value" : 2.0
												}
,
												"mode" : 												{
													"value" : 1.0
												}
,
												"end" : 												{
													"value" : 1.0
												}
,
												"rate" : 												{
													"value" : 1.0
												}

											}
,
											"layer2" : 											{
												"xfade" : 												{
													"value" : 1.0
												}
,
												"gain" : 												{
													"value" : 1.0
												}
,
												"loop" : 												{
													"value" : 1.0
												}
,
												"begin" : 												{
													"value" : 0.0
												}
,
												"size" : 												{
													"value" : 10.0
												}
,
												"mode" : 												{
													"value" : 0.0
												}
,
												"end" : 												{
													"value" : 1.0
												}
,
												"rate" : 												{
													"value" : 1.0
												}

											}
,
											"layer1" : 											{
												"xfade" : 												{
													"value" : 1.0
												}
,
												"gain" : 												{
													"value" : 1.0
												}
,
												"loop" : 												{
													"value" : 1.0
												}
,
												"begin" : 												{
													"value" : 0.0
												}
,
												"size" : 												{
													"value" : 3.0
												}
,
												"mode" : 												{
													"value" : 1.0
												}
,
												"end" : 												{
													"value" : 1.0
												}
,
												"rate" : 												{
													"value" : 1.0
												}

											}

										}
,
										"rec-begin" : 										{
											"value" : 0.0
										}
,
										"rec-mode" : 										{
											"value" : 0.0
										}
,
										"rec-loop" : 										{
											"value" : 1.0
										}
,
										"rec-gain" : 										{
											"value" : 1.0
										}
,
										"rec-mon" : 										{
											"value" : 100.0
										}
,
										"rec-end" : 										{
											"value" : 0.0
										}
,
										"rec-layer" : 										{
											"value" : 0.0
										}
,
										"__presetid" : "loop"
									}
,
									"fileref" : 									{
										"name" : "loop",
										"filename" : "loop.maxsnap",
										"filepath" : "~/Documents/Max 8/Snapshots",
										"filepos" : -1,
										"snapshotfileid" : "e723a0a6eeb612af5d362c01f5d6330d"
									}

								}
 ]
						}

					}
,
					"text" : "rnbo~ loop",
					"varname" : "rnbo~"
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-9",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 944.0, 573.0, 163.0, 127.0 ],
					"trigger" : 1
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-8",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 755.0, 573.0, 163.0, 127.0 ],
					"trigger" : 1
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-5",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 566.0, 573.0, 163.0, 127.0 ],
					"trigger" : 1
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-4",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 377.0, 573.0, 163.0, 127.0 ],
					"trigger" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 944.0, 543.0, 129.0, 22.0 ],
					"text" : "buffer~ layer4 10000 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 755.0, 543.0, 129.0, 22.0 ],
					"text" : "buffer~ layer3 10000 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 566.0, 543.0, 129.0, 22.0 ],
					"text" : "buffer~ layer2 10000 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 377.0, 543.0, 129.0, 22.0 ],
					"text" : "buffer~ layer1 10000 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 386.0, 141.0, 66.0, 22.0 ],
					"text" : "cycle~ 440"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 476.0, 141.0, 64.0, 22.0 ],
					"text" : "saw~ 440\\,"
				}

			}
, 			{
				"box" : 				{
					"bufsize" : 256,
					"calccount" : 2,
					"id" : "obj-3",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 198.0, 573.0, 163.0, 127.0 ],
					"trigger" : 1
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-mon",
					"id" : "obj-14",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 60.0, 224.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-layer",
					"id" : "obj-2",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 60.0, 156.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-mode",
					"id" : "obj-22",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 60.0, 184.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/mode",
					"id" : "obj-18",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 620.0, 124.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer2/mode",
					"id" : "obj-19",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 620.0, 286.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer3/mode",
					"id" : "obj-1",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 620.0, 322.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer4/mode",
					"id" : "obj-15",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 620.0, 359.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-gain",
					"id" : "obj-16",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 60.0, 254.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/begin",
					"id" : "obj-24",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 620.0, 166.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/end",
					"id" : "obj-25",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 772.0, 166.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/rate",
					"id" : "obj-26",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 772.0, 124.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/gain",
					"id" : "obj-27",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 772.0, 197.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/loop",
					"id" : "obj-28",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 944.0, 124.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-loop",
					"id" : "obj-35",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 60.0, 296.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-begin",
					"id" : "obj-36",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 56.0, 384.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "rec-end",
					"id" : "obj-39",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 56.0, 434.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/xfade",
					"id" : "obj-21",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 620.0, 197.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer2/end",
					"id" : "obj-44",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1019.0, 286.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer2/begin",
					"id" : "obj-45",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 791.5, 286.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer2/rate",
					"id" : "obj-46",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1264.0, 286.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "layer1/size",
					"id" : "obj-53",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1336.0, 405.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-71",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1303.0, 625.5, 63.0, 22.0 ],
					"text" : "setsize $1"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-14", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 0 ],
					"order" : 0,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"source" : [ "obj-17", 5 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"order" : 1,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-30", 0 ],
					"source" : [ "obj-17", 6 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"source" : [ "obj-17", 7 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"source" : [ "obj-17", 8 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-34", 0 ],
					"source" : [ "obj-17", 9 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"source" : [ "obj-17", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"source" : [ "obj-17", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"source" : [ "obj-17", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-17", 4 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-23", 1 ],
					"order" : 0,
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-23", 0 ],
					"order" : 1,
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-36", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-39", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 5 ],
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 6 ],
					"source" : [ "obj-41", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 3 ],
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-48", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"order" : 3,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"order" : 2,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"order" : 1,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"order" : 0,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 4 ],
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-53", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 1 ],
					"source" : [ "obj-54", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-53", 0 ],
					"order" : 0,
					"source" : [ "obj-57", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-75", 0 ],
					"order" : 1,
					"source" : [ "obj-57", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 2 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-64", 0 ],
					"order" : 0,
					"source" : [ "obj-63", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-77", 0 ],
					"order" : 1,
					"source" : [ "obj-63", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-64", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 0 ],
					"order" : 0,
					"source" : [ "obj-67", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-73", 0 ],
					"order" : 1,
					"source" : [ "obj-67", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-68", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-70", 0 ],
					"order" : 0,
					"source" : [ "obj-69", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-72", 0 ],
					"order" : 1,
					"source" : [ "obj-69", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 1 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-70", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"source" : [ "obj-71", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-71", 0 ],
					"source" : [ "obj-72", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-74", 0 ],
					"source" : [ "obj-73", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"source" : [ "obj-74", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-76", 0 ],
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"source" : [ "obj-76", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"source" : [ "obj-77", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-78", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-17" : [ "rnbo~", "rnbo~", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "loop.maxsnap",
				"bootpath" : "~/Documents/Max 8/Snapshots",
				"patcherrelativepath" : "../../../../../Documents/Max 8/Snapshots",
				"type" : "mx@s",
				"implicit" : 1
			}
, 			{
				"name" : "loop.rnbopat",
				"bootpath" : "~/projects/SSP.private/technobear/loop/rnbo-patch",
				"patcherrelativepath" : ".",
				"type" : "RBOP",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
