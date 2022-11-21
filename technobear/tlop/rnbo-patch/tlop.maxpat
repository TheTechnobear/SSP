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
		"rect" : [ 83.0, 197.0, 1136.0, 1068.0 ],
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
					"numinlets" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 319.0, 235.0, 68.0, 22.0 ],
					"text" : "selector~ 2"
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
 ]
					}
,
					"maxclass" : "newobj",
					"numinlets" : 1,
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
								"comment" : "record head"
							}
, 							{
								"type" : "signal",
								"index" : 7,
								"tag" : "out7",
								"comment" : "layer1-play"
							}
, 							{
								"type" : "signal",
								"index" : 8,
								"tag" : "out8",
								"comment" : "layer2-play"
							}
, 							{
								"type" : "signal",
								"index" : 9,
								"tag" : "out9",
								"comment" : "layer3-play"
							}
, 							{
								"type" : "signal",
								"index" : 100,
								"tag" : "out100",
								"comment" : "layer4-play"
							}
 ]
					}
,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "list" ],
					"patching_rect" : [ 319.0, 365.0, 213.0, 22.0 ],
					"rnboattrcache" : 					{
						"layer1/rate" : 						{
							"label" : "rate",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer4/crossfade" : 						{
							"label" : "crossfade",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer1/crossfade" : 						{
							"label" : "crossfade",
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
						"layer2/end" : 						{
							"label" : "end",
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
						"layer3/gain" : 						{
							"label" : "gain",
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
						"layer1/begin" : 						{
							"label" : "begin",
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
						"layer2/rate" : 						{
							"label" : "rate",
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
						"layer2/begin" : 						{
							"label" : "begin",
							"isEnum" : 0,
							"parsestring" : ""
						}
,
						"layer3/crossfade" : 						{
							"label" : "crossfade",
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
						"layer" : 						{
							"label" : "layer",
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
						"rec_mon" : 						{
							"label" : "rec_mon",
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
						"layer2/crossfade" : 						{
							"label" : "crossfade",
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
						"layer3/end" : 						{
							"label" : "end",
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
						"record-mode" : 						{
							"label" : "record-mode",
							"isEnum" : 1,
							"parsestring" : "Stop Record"
						}
,
						"layer1/mode" : 						{
							"label" : "mode",
							"isEnum" : 1,
							"parsestring" : "Play Stop"
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
							"parsestring" : "Play Stop"
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
							"parsestring" : "Play Stop"
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
							"parsestring" : "Play Stop"
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
							"record-mode" : 							{
								"value" : 0.0
							}
,
							"rec-gain" : 							{
								"value" : 1.0
							}
,
							"layer" : 							{
								"value" : 0.0
							}
,
							"__sps" : 							{
								"layer4" : 								{
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
									"mode" : 									{
										"value" : 1.0
									}
,
									"crossfade" : 									{
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
								"layer3" : 								{
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
									"mode" : 									{
										"value" : 1.0
									}
,
									"crossfade" : 									{
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
									"mode" : 									{
										"value" : 1.0
									}
,
									"crossfade" : 									{
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
								"layer1" : 								{
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
									"mode" : 									{
										"value" : 0.0
									}
,
									"crossfade" : 									{
										"value" : 1.0
									}
,
									"end" : 									{
										"value" : 0.0
									}
,
									"rate" : 									{
										"value" : 1.0
									}

								}

							}
,
							"rec_mon" : 							{
								"value" : 100.0
							}
,
							"__presetid" : "tlop"
						}
,
						"snapshotlist" : 						{
							"current_snapshot" : 0,
							"entries" : [ 								{
									"filetype" : "C74Snapshot",
									"version" : 2,
									"minorversion" : 0,
									"name" : "tlop",
									"origin" : "tlop",
									"type" : "rnbo",
									"subtype" : "",
									"embed" : 0,
									"snapshot" : 									{
										"record-mode" : 										{
											"value" : 0.0
										}
,
										"rec-gain" : 										{
											"value" : 1.0
										}
,
										"layer" : 										{
											"value" : 0.0
										}
,
										"__sps" : 										{
											"layer4" : 											{
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
												"mode" : 												{
													"value" : 1.0
												}
,
												"crossfade" : 												{
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
											"layer3" : 											{
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
												"mode" : 												{
													"value" : 1.0
												}
,
												"crossfade" : 												{
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
												"mode" : 												{
													"value" : 1.0
												}
,
												"crossfade" : 												{
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
											"layer1" : 											{
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
												"mode" : 												{
													"value" : 0.0
												}
,
												"crossfade" : 												{
													"value" : 1.0
												}
,
												"end" : 												{
													"value" : 0.0
												}
,
												"rate" : 												{
													"value" : 1.0
												}

											}

										}
,
										"rec_mon" : 										{
											"value" : 100.0
										}
,
										"__presetid" : "tlop"
									}
,
									"fileref" : 									{
										"name" : "tlop",
										"filename" : "tlop.maxsnap",
										"filepath" : "~/Documents/Max 8/Snapshots",
										"filepos" : -1,
										"snapshotfileid" : "58e8805571548faa66a3891b3db5e4fb"
									}

								}
 ]
						}

					}
,
					"text" : "rnbo~ tlop",
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
					"attr" : "rec_mon",
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
					"attr" : "layer",
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
					"attr" : "record-mode",
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
					"attr" : "layer1/crossfade",
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
					"destination" : [ "obj-31", 2 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 1 ],
					"source" : [ "obj-7", 0 ]
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
				"name" : "tlop.maxsnap",
				"bootpath" : "~/Documents/Max 8/Snapshots",
				"patcherrelativepath" : "../../../../../Documents/Max 8/Snapshots",
				"type" : "mx@s",
				"implicit" : 1
			}
, 			{
				"name" : "tlop.rnbopat",
				"bootpath" : "~/projects/SSP.private/technobear/tlop/rnbo-patch",
				"patcherrelativepath" : ".",
				"type" : "RBOP",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
