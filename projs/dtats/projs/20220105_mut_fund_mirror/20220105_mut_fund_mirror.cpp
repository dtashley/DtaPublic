//----------------------------------------------------------------------------------------------------
//I discovered in early 2022 that most of the mutual funds I owned had substantially the same major
//holdings, so that I was getting oversized exposure to certain large and favorable companies.
//I decided instead to sell all my mutual funds and replace them with fixed-dollar positions of the
//most popular holdings (this eliminated the heavy redundancy between funds, as I would have only
//one position of each underlying stock).
//
//The input of this program is the most prominent (publicly available) holdings of each mutual fund
//that I'd like to consider as part of the algorithm, and the output is a list of all stocks held
//by any of the mutual funds, ranked in order of "popularity" among the mutual funds.
//
//"Popularity" is determined as follows.
//   a)The first stock is weighted with a max, linearly down through the last stock with a min.
//   b)Multiplied by the ARR squared of the mutual fund.
//   c)The popularities for each stock are added together across all mutual funds, to give a total
//     popularity for that stock.
//
//This is essentially a C program processed through a C compiler.  Quick.  Dirty.  Etc.
//----------------------------------------------------------------------------------------------------
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const double total_funds = 80000.0;
   //Total money available to spend.
static const double weight_max  = 2.0;
   //Weighting of first stock in list for mutual fund.
static const double weight_min  = 1.0;
   //Weighting of last stock in list for mutual fund.
static const double round_increment = 100.0;
   //The granularity with which stocks are bought.
static const double min_buy_amount = 100.0;
   //Minimum amount of stock to buy.
static const size_t max_n_stocks = 200;
   //The maximum number of stocks to own.
static const int diag_output = 0;
   //1 for debugging output.


struct StMutualFundHolding
{
   const char* symbol; //Must not be NULL or empty string.
};

struct StMutualFund
{
   const char* symbol; //Must not be NULL.  May be empty string.
   const char* desc;   //Must not be NULL.  May be empty string.
   const double arr;
   const struct StMutualFundHolding* holdings;
   size_t n_holdings;
};

struct StMutualFundHoldingLookup
{
   const char* symbol;  //Must not be NULL or empty string.
   const char* desc;    //Must not be NULL or empty string.
};

struct StatisticsTableRow
{
   size_t index;
   double popularity;
   double funds_allocated;
   double funds_allocated_rounded;
};


//Master lookup table that gives correspondence between stock symbols and company or equity
//names.
static const struct StMutualFundHoldingLookup holding_lookup[] =
{
   { "AAPL",   "Apple Inc."                                       },
   { "ABBV",   "AbbVie Inc."                                      },
   { "ABNB",   "Airbnb Inc. Ordinary Shares Class A"              },
   { "ABT",    "Abbott Laboratories"                              },
   { "ACGL",   "Arch Capital Group Ltd."                          },
   { "ACN",    "Accenture PLC Class A"                            },
   { "ADBE",   "Adobe Inc."                                       },
   { "ADI",    "Analog Devices Inc."                              },
   { "ADM",    "Archer-Daniels-Midland Co."                       },
   { "ADP",    "Automatic Data Processing Inc."                   },
   { "ADSK",   "Autodesk Inc."                                    },
   { "ADYEY",  "Adyen NV"                                         },
   { "AEO",    "American Eagle Outfitters Inc."                   },
   { "AEP",    "American Electric Power Co. Inc."                 },
   { "AES",    "AES Corporation"                                  },
   { "AGCO",   "AGCO Corporation"                                 },
   { "AGO",    "Assured Guaranty Ltd."                            },
   { "AIG",    "American International Group Inc."                },
   { "AJG",    "Arthur J. Gallagher & Co."                        },
   { "AIZ",    "Assurant Inc."                                    },
   { "AKAM",   "Akamai Technologies Inc."                         },
   { "ALGN",   "Align Technology Inc."                            },
   { "ALIT",   "Alight Inc. Class A"                              },
   { "ALL",    "Allstate Corporation"                             },
   { "ALNY",   "Alnylam Pharmaceuticals Inc."                     },
   { "AMAT",   "Applied Materials Inc."                           },
   { "AMD",    "Advanced Micro Devices Inc."                      },
   { "AMGN",   "Amgen Inc."                                       },
   { "AMT",    "American Tower Corporation"                       },
   { "AMZN",   "Amazon.com Inc."                                  },
   { "ANET",   "Arista Networks Inc."                             },
   { "AON",    "Aon PLC Class A"                                  },
   { "APH",    "Amphenol Corporation Class A"                     },
   { "APLS",   "Apellis Pharmaceuticals Inc."                     },
   { "APO",    "Apollo Global Managment Inc. Class A"             },
   { "AQUA",   "Evoqua Water Technologies Corporation"            },
   { "AR",     "Antero Resources Corp."                           },
   { "ARES",   "Ares Management Corporation Class A"              },
   { "ARGX",   "argenx SE ADR",                                   },
   { "ARVN",   "Arvinas Inc."                                     },
   { "ARWR",   "Arrowhead Pharmaceuticals Inc."                   },
   { "ASML",   "ASML Holding NV ADR"                              },
   { "ASND",   "Ascendis Pharma A/S ADR"                          },
   { "ATH",    "Athene Holding Ltd."                              },
   { "AVGO",   "Broadcon Inc."                                    },
   { "AVTR",   "Avantor Inc."                                     },
   { "AWK",    "American Water Works Company Inc."                },
   { "AWR",    "American States Water Company"                    },
   { "AZN",    "AstraZeneca PLC"                                  },
   { "BAC",    "Bank of America Corporation"                      },
   { "BALY",   "Bally's Corporation"                              },
   { "BBWI",   "Bath & Body Works Inc."                           },
   { "BG",     "Bunge Ltd."                                       },
   { "BHVN",   "Biohaven Pharmaceutical Holding Co. Ltd."         },
   { "BIO",    "Bio-Rad Laboratories Inc. Class A"                },
   { "BKNG",   "Booking Holdings Inc."                            },
   { "BJ",     "BJ's Wholesale Club Holdings Inc."                },
   { "BKFKF",  "Bakkafrost P/F"                                   },
   { "BLDR",   "Builders FirstSource Inc."                        },
   { "BLK",    "Blackrock Inc."                                   },
   { "BMI",    "Badger Meter Inc."                                },
   { "BMY",    "Bristol-Myers Squibb Co."                         },
   { "BPMC",   "Blueprint Medicines Corporation",                 },
   { "BRKB",   "Berkshire Hathaway Inc. Class B"                  },
   { "BRKR",   "Bruker Corporation",                              },
   { "BRO",    "Brown & Brown Inc."                               },
   { "BRP",    "BRP Group Inc. Class A",                          },
   { "BSX",    "Boston Scientific Corporation"                    },
   { "BURL",   "Burlington Stores Inc."                           },
   { "BWI",    "Badger Meter Inc."                                },
   { "CB",     "Chubb Ltd."                                       },
   { "CCXI",   "ChemoCentryx Inc."                                },
   { "CDNS",   "Cadence Design Systems Inc."                      },
   { "CF",     "CF Industries Holdings Inc."                      },
   { "CGEMY",  "Capgemini SE"                                     },
   { "CHDN",   "Churchill Downs Inc."                             },
   { "CHK",    "Chesapeake Energy Ordinary Shares"                },
   { "CI",     "Cigna Corporation"                                },
   { "CLVT",   "Clarivate PLC Ordinary Shares"                    },
   { "CMCSA",  "Comcast Corporation Class A"                      },
   { "CMG",    "Chipotle Mexican Grill"                           },
   { "CNC",    "Centene Corporation"                              },
   { "CNHI",   "CNH Industrial NV"                                },
   { "CNP",    "CenterPoint Energy Inc."                          },
   { "COF",    "Capital One Financial Corporation"                },
   { "COST",   "Costco Wholesale Corporation"                     },
   { "CPRI",   "Capri Holdings Ltd."                              },
   { "CR",     "Crane Co."                                        },
   { "CRC",    "California Resources Corporation"                 },
   { "CRL",    "Charles River Laboratories International Inc."    },
   { "CRM",    "Salesforce.com Inc."                              },
   { "CROX",   "Crocs Inc."                                       },
   { "CRUS",   "Cirrus Logic Inc."                                },
   { "CSCO",   "Cisco Systems Inc."                               },
   { "CTAS",   "Cintas Corporation"                               },
   { "CTSH",   "Cognizant Technology Solutions Class A"           },
   { "CTVA",   "Corteva Inc."                                     },
   { "CUBE",   "CubeSmart"                                        },
   { "CVNA",   "Carvana Company Class A"                          },
   { "CVX",    "Chevron Corporation"                              },
   { "CWEN",   "Clearway Energy Inc. Class C"                     },
   { "CWK",    "Cranswick PLC"                                    },
   { "CYBR",   "Cyberark Software Ltd."                           },
   { "CYTK",   "Cytokinetics Inc."                                },
   { "CZR",    "Caesars Entertainment Inc."                       },
   { "D",      "Dominion Energy Inc."                             },
   { "DAR",    "Darling Ingredients Inc."                         },
   { "DE",     "Deere & Co."                                      },
   { "DG",     "Dollar General Corporation"                       },
   { "DHR",    "Danaher Corporation"                              },
   { "DIOD",   "Diodes Inc."                                      },
   { "DLR",    "Digital Realty Trust Inc."                        },
   { "DLTR",   "Dollar Tree Inc."                                 },
   { "DOW",    "Dow Inc."                                         },
   { "DPZ",    "Domino's Pizza Inc."                              },
   { "DXC",    "DXC Technology Co."                               },
   { "DUK",    "Duke Energy Corporation"                          },
   { "DXCM",   "DexCom Inc."                                      },
   { "EAT",    "Brinker International Inc."                       },
   { "EBAY",   "eBay Inc."                                        },
   { "ECL",    "Ecolab Inc."                                      },
   { "EIX",    "Edison International"                             },
   { "EL",     "Estee Lauder Companies Inc."                      },
   { "EPAM",   "EPAM Systems Inc."                                },
   { "EPEGQ",  "EP Energy Corporation Class A"                    },
   { "EQIX",   "Equinix Inc."                                     },
   { "ESS",    "Essex Property Trust Inc."                        },
   { "ESTC",   "Elastic NV"                                       },
   { "ETR",    "Entergy Corporation"                              },
   { "EVA",    "Enviva Partners LP"                               },
   { "EVRG",   "Evergy Inc."                                      },
   { "EXC",    "Exelon Corporation"                               },
   { "FAF",    "First American Financial Corporation"             },
   { "FB",     "Meta Platforms Inc. Class A"                      },
   { "FE",     "FirstEnergy Corporation"                          },
   { "FI-N",   "Georg Fischer AG"                                 },
   { "FIS",    "Fidelity National Information Services Inc."      },
   { "FISV",   "Fiserv Inc."                                      },
   { "FIVE",   "Five Below Inc."                                  },
   { "FLEX",   "Flex Ltd."                                        },
   { "FMC",    "FMC Corporation"                                  },
   { "FND",    "Floor & Decor Holdings Inc. Class A"              },
   { "FTCH",   "Farfetch Ltd. Class A"                            },
   { "FTROF",  "First Resources Ltd."                             },
   { "FWONA",  "Liberty Media Formula One Series A"               },
   { "G",      "Genpact Ltd."                                     },
   { "GFS",    "Globalfoundries Inc."                             },
   { "GILD",   "Gilead Sciences Inc."                             },
   { "GLBE",   "Global-E Online Ltd."                             },
   { "GOOG",   "Alphabet Inc. Class C"                            },
   { "GOOGL",  "Alphabet Inc. Class A"                            },
   { "GPN",    "Global Payments Inc."                             },
   { "HBAN",   "Huntington Bancshares Inc."                       },
   { "HCA",    "HCA Healthcare Inc."                              },
   { "HD",     "The Home Depot Inc."                              },
   { "HIG",    "The Hartford Financial Services Group Inc."       },
   { "HLMAF",  "Halma PLC"                                        },
   { "HLT",    "Hilton Worldwide Holdings Inc."                   },
   { "HOLX",   "Hologic Inc."                                     },
   { "HTHIY",  "Hitachi Ltd."                                     },
   { "HUBS",   "Hubspot Inc."                                     },
   { "HUM",    "Humana Inc."                                      },
   { "HZNP",   "Horizon Therapeutics PLC",                        },
   { "IBM",    "IBM Common Stock"                                 },
   { "IEX",    "IDEX Corporation"                                 },
   { "INDT",   "Indutrade AB"                                     },
   { "INGR",   "Ingredion Inc."                                   },
   { "INSP",   "Inspire Medical Systems Inc."                     },
   { "INTC",   "Intel Corporation"                                },
   { "INTU",   "Intuit Inc."                                      },
   { "IQV",    "IQVIA Holdings Inc."                              },
   { "IR",     "Ingersoll Rand Inc."                              },
   { "ISRG",   "Intuitive Surgical Inc."                          },
   { "IT",     "Gartner Inc."                                     },
   { "JBL",    "Jabil Inc."                                       },
   { "JBSAY",  "JBS ADR"                                          },
   { "JNJ",    "Johnson & Johnson"                                },
   { "JPM",    "JPMorgan Chase & Co."                             },
   { "JWN",    "Nordstrom Inc."                                   },
   { "KBR",    "KBR Inc."                                         },
   { "KLAC",   "KLA Corporation"                                  },
   { "KO",     "Coco-Cola Co."                                    },
   { "KRYS",   "Krystal Biotech Inc."                             },
   { "KSS",    "Kohl's Corporation"                               },
   { "KUBTY",  "Kubota Corporation"                               },
   { "L",      "Loews Corporation"                                },
   { "LAMR",   "Lamar Advertising Co. Class A"                    },
   { "LIN",    "Linde PLC"                                        },
   { "LIND",   "Lindblad Expeditions Holdings Inc."               },
   { "LLY",    "Eli Lilly and Co."                                },
   { "LOVE",   "Lovesac Co."                                      },
   { "LOW",    "Lowe's Companies Inc."                            },
   { "LRCX",   "Lam Research Corp."                               },
   { "LULU",   "Lululemon Athletica Inc."                         },
   { "LVS",    "Las Vegas Sands Corporation"                      },
   { "LW",     "Lamb Weston Holdings Inc."                        },
   { "LYV",    "Live Nation Entertainment Inc."                   },
   { "LZAGY",  "Lonza Group Ltd. ADR"                             },
   { "MA",     "Mastercard Inc. Class A"                          },
   { "MAA",    "Mid-America Apartment Communities Inc."           },
   { "MAR",    "Marriott International Inc."                      },
   { "MASI",   "Masimo Corporation."                              },
   { "MCD",    "McDonald's Corporation"                           },
   { "MCHP",   "Microchip Technology Inc."                        },
   { "MDB",    "MongoDB Inc. Class A"                             },
   { "MEG",    "Montrose Environmental Group Inc."                },
   { "MET",    "MetLife Inc."                                     },
   { "MELI",   "Mercadolibre Inc."                                },
   { "MHGVY",  "Mowi ASA Sponsored ADR"                           },
   { "MMC",    "Marsh & McLennan Companies Inc."                  },
   { "MMM",    "3M Co."                                           },
   { "MNTV",   "Momentive Global Inc."                            },
   { "MOS",    "The Mosaic Co."                                   },
   { "MRK",    "Merck & Co. Inc."                                 },
   { "MRNA",   "Moderna Inc."                                     },
   { "MRTX",   "Mirati Therapeutics Inc."                         },
   { "MRVL",   "Marvell Technology Inc."                          },
   { "MSEX",   "Middlesex Water Company"                          },
   { "MSFT",   "Microsoft Corporation"                            },
   { "MTN",    "Vail Resorts Inc."                                },
   { "MTSI",   "MACOM Technology Solutions Holdings Inc."         },
   { "MWA",    "Mueller Water Products Inc. Class A"              },
   { "NEE",    "NextEra Energy Inc."                              },
   { "NEP",    "NextEra Energy Partners LP"                       },
   { "NET",    "Cloudflare Inc."                                  },
   { "NFLX",   "Netflix Inc."                                     },
   { "NI",     "NiSource Inc."                                    },
   { "NKE",    "Nike Inc. Class B"                                },
   { "NOMD",   "Nomad Foods Ltd."                                 },
   { "NOVA",   "Sunnova Energy International Inc."                },
   { "NRG",    "NRG Energy Inc."                                  },
   { "NTLA",   "Intellia Therapeutics Inc."                       },
   { "NTR",    "Nutrien Ltd."                                     },
   { "NVDA",   "NVIDIA Corporation"                               },
   { "NVMI",   "Nova Ltd."                                        },
   { "NVST",   "Envista Holdings Corporation"                     },
   { "NXPI",   "NXP Semiconductors NV"                            },
   { "OGE",    "OGE Energy Corporation"                           },
   { "OKTA",   "OKTA Inc."                                        },
   { "OLLI",   "Ollie's Bargain Outlet Holdings Inc."             },
   { "OLN",    "Olin Corporation"                                 },
   { "ON",     "On Semiconductor Corp."                           },
   { "ORCL",   "Oracle Corporation"                               },
   { "PANW",   "Palo Alto Networks Inc."                          },
   { "PATH",   "UiPath Inc."                                      },
   { "PCG",    "PG&E Corporation"                                 },
   { "PDYPY",  "Flutter Entertainment PLC ADR"                    },
   { "PEG",    "Public Service Enterprise Group Inc."             },
   { "PEN",    "Penumbra Inc."                                    },
   { "PEP",    "PepsiCo Inc."                                     },
   { "PFG",    "Principal Financial Group Inc."                   },
   { "PG",     "Procter & Gamble Co."                             },
   { "PGR",    "Progressive Corporation"                          },
   { "PLD",    "Prologic Inc."                                    },
   { "PNR",    "Pentair PLC"                                      },
   { "PODD",   "Insulet Corp."                                    },
   { "PRU",    "Prudentialk Financial Inc."                       },
   { "PRCH",   "Porch Group Inc. Ordinary Shares Class A"         },
   { "PRI",    "Primerica Inc."                                   },
   { "PRU",    "Prudentialk Financial Inc."                       },
   { "PTCT",   "PTC Therapeutics Inc."                            },
   { "PVH",    "PVH Corporation"                                  },
   { "PYPL",   "Paypal Holdings Inc."                             },
   { "QCOM",   "QUALCOMM, Inc."                                   },
   { "QSR",    "Restaurant Brands International Inc."             },
   { "RARE",   "Ultragenyx Pharmaceutical Inc."                   },
   { "RCUS",   "Arcus Biosciences Inc."                           },
   { "REGN",   "Regeneron Pharmaceuticals Inc."                   },
   { "RGA",    "Reinsurance Group of America, Inc."               },
   { "RHHBY",  "Roche Holdings AG Basel ADR Common Stock"         },
   { "RLNIY",  "Reliance Industries, Ltd."                        },
   { "RMD",    "ResMed Inc."                                      },
   { "ROP",    "Roper Technologies Inc."                          },
   { "ROST",   "Ross Stores Inc."                                 },
   { "RPD",    "Rapid7 Inc."                                      },
   { "RPRX",   "Royalty Pharma PLC Class A"                       },
   { "RUN",    "Sunrun Inc."                                      },
   { "RUTH",   "Ruth's Hospitality Group Inc."                    },
   { "SBNY",   "Signature Bank"                                   },
   { "SBUX",   "Starbucks Corporation"                            },
   { "SCPPF",  "S4 Capital PLC"                                   },
   { "SDMHF",  "Sartorius Stedim Biotech SA"                      },
   { "SE",     "Sea Ltd."                                         },
   { "SGEN",   "Seagen Inc. Ordinary Shares"                      },
   { "SITM",   "SiTime Corp. Ordinary Shares"                     },
   { "SMG",    "The Scotts Miracle Gro Co. Class A"               },
   { "SMPL",   "The Simply Good Foods Co."                        },
   { "SMTC",   "Semtech Corp."                                    },
   { "SNAP",   "Snap Inc."                                        },
   { "SNOW",   "Snowflake Inc."                                   },
   { "SO",     "Southern Company"                                 },
   { "SPT",    "Sprout Social Inc. Class A"                       },
   { "SQ",     "Block Inc."                                       },
   { "SRE",    "Sempra Energy"                                    },
   { "STRNY",  "Severn Trent ADR"                                 },
   { "SYK",    "Stryker Corporation"                              },
   { "SYNH",   "Syneos Health Inc. Class A"                       },
   { "TCEHY",  "Tencent Holdings Ltd. ADR"                        },
   { "TDY",    "Teledyne Technologies Inc."                       },
   { "TENB",   "Tenable Holdings Inc."                            },
   { "TER",    "Teradyne Inc."                                    },
   { "TGT",    "Target Corporation"                               },
   { "TITN",   "Titan Machinery Inc."                             },
   { "TJX",    "TJX Companies Inc."                               },
   { "TMO",    "Thermo Fisher Scientific Inc."                    },
   { "TMUS",   "T-Mobile Us Inc."                                 },
   { "TNDM",   "Tandem Diabetes Care Inc."                        },
   { "TPR",    "Tapestry Inc."                                    },
   { "TPX",    "Tempur Sealy International Inc."                  },
   { "TRV",    "The Travelers Companies Inc."                     },
   { "TSLA",   "Tesla Inc."                                       },
   { "TSM",    "Taiwan Semiconductor Mfg. Company Ltd."           },
   { "TTC",    "The Toro Co."                                     },
   { "TTEK",   "Tetra Tech Inc."                                  },
   { "TTGT",   "TechTarget Inc."                                  },
   { "TWLO",   "Twilio Inc. Class A"                              },
   { "TWTR",   "Twitter Inc."                                     },
   { "TXG",    "10X Genomics Inc."                                },
   { "TXN",    "Texas Instruments Inc."                           },
   { "UBER",   "Uber Technologies Inc."                           },
   { "ULTA",   "Ulta Beauty Inc."                                 },
   { "UNH",    "UnitedHealth Group Inc."                          },
   { "UPS",    "United Parcel Service Inc. Class B"               },
   { "USFD",   "US Foods Holding Corporation"                     },
   { "UTHR",   "United Therapeutics Corporation"                  },
   { "V",      "Visa Inc. Class A"                                },
   { "VAC",    "Marriott Vacations Worldwide Corporation"         },
   { "VEOEY",  "Veolia Environnement SA"                          },
   { "VRSK",   "Verisk Analytics Inc."                            },
   { "VRTX",   "Vertex Pharmaceuticals Inc."                      },
   { "VST",    "Vistra Corporation"                               },
   { "VTR",    "Ventas Inc."                                      },
   { "VVV",    "Valvoline Inc."                                   },
   { "VZ",     "Verizon Communications Inc."                      },
   { "WBA",    "Walgreens Boots Alliance Inc."                    },
   { "WDAY",   "Workday Inc. Class A"                             },
   { "WLMIF",  "Wilmar International Ltd"                         },
   { "WLTW",   "Willis Towers Watson PLC"                         },
   { "WMS",    "Advanced Drainage Systems Inc."                   },
   { "WNS",    "WNS (Holdings) Ltd. ADR"                          },
   { "WSC",    "Willscot Mobile Mini Holdings Corporation"        },
   { "WST",    "West Pharmaceutical Services Inc."                },
   { "WTS",    "Watts Water Technologies Inc. Class A"            },
   { "YUM",    "Yum! Brands Inc."                                 },
   { "XLNX",   "Xilinx Inc."                                      },
   { "XYL",    "Xylen Inc."                                       },
   { "ZEN",    "Zendesk Inc."                                     },
   { "ZNTL",   "Zentalis Pharmaceuticals Inc. Ordinary Shares"    },
   { "ZS",     "Zscaler Inc."                                     },
   { "ZWS",    "Zurn Water Solutions Corporation"                 },
   };


//Dogs of the Dow 2022 Strategy Stocks.
static const struct StMutualFundHolding dotd_holdings[] =
{
   "DOW",
   "VZ",
   "IBM",
   "CVX",
   "WBA",
   "MRK",
   "AMGN",
   "MMM",
   "KO",
   "INTC"
};

//Holdings of FAGIX (Fidelity Capital and Income).
static const struct StMutualFundHolding fagix_holdings[] =
{
   "CRC",
   "CZR",
   "GOOGL",
   "MSFT",
   "CHK",
   "TPX",
   "IQV",
   "FB",
   "ADBE",
   "JBSAY",
   "DAR",
   "LRCX",
   "EPEGQ",
};


//Holdings of FARMX (Fidelity Agricultural Productivity).
static const struct StMutualFundHolding farmx_holdings[] =
{
   "DE",
   "NTR",
   "CTVA",
   "ADM",
   "CF",
   "MOS",
   "BG",
   "CNHI",
   "DAR",
   "TTC",
   "KUBTY",
   "AGCO",
   "LW",
   "SMPL",
   "MHGVY",
   "BKFKF",
   "INGR",
   "WLMIF",
   "SMG",
   "TITN",
   "FMC",
   "CWK",
   "FTROF",
   "NOMD",
};


//Holdings of FBIOX (Fidelity Select Biotechnology).
static const struct StMutualFundHolding fbiox_holdings[] =
{
   "MRNA",
   "GILD",
   "ALNY",
   "ABBV",
   "VRTX",
   "REGN",
   "SGEN",
   "RCUS",
   "BHVN",
   "ARGX",
   "KRYS",
   "ARVN",
   "HZNP",
   "NTLA",
   "BPMC",
   "MRTX",
   "CYTK",
   "ASND",
   "PTCT",
   "RARE",
   "ARWR",
   "UTHR",
   "APLS",
   "CCXI",
};


//Holdings of FBSOX (Fidelity Select IT Services).
static const struct StMutualFundHolding fbsox_holdings[] =
{
   "MA",
   "V",
   "PYPL",
   "ACN",
   "SQ",
   "INTU",
   "CTSH",
   "TWLO",
   "FIS",
   "FISV",
   "EPAM",
   "DXC",
   "WNS",
   "SNOW",
   "OKTA",
   "MDB",
   "G",
   "GPN",
   "SCPPF",
   "FB",
   "ADP",
   "GLBE",
   "ALIT",
   "IT",
   "PATH",
};


//Holdings of FCNTX (Fidelity Contrafund).
static const struct StMutualFundHolding fcntx_holdings[] =
{
   "FB",
   "AMZN",
   "MSFT",
   "BRKB",
   "NVDA",
   "AAPL",
   "GOOGL",
   "UNH",
   "GOOG",
   "CRM",
   "ADBE",
   "NFLX",
   "APH",
   "V",
   "QCOM",
   "HD",
   "COST",
   "LLY",
   "BAC",
   "JPM",
   "EL",
   "AMD",
   "NET",
   "REGN",
};


//Holdings of FCPGX (Fidelity Small Cap Growth).
static const struct StMutualFundHolding fcpgx_holdings[] =
{
   "CROX",
   "SITM",
   "TTGT",
   "BJ",
   "PODD",
   "BLDR",
   "RPD",
   "KBR",
   "TENB",
   "CZR",
   "LOVE",
   "VVV",
   "AEO",
   "LIND",
   "CYBR",
   "SPT",
   "SMTC",
   "ESTC",
   "WSC",
   "CHDN",
   "SYNH",
   "AR",
   "CR",
   "PRCH",
   "EVA",
};


//Holdings of FDLSX (Fidelity Select Leisure).
static const struct StMutualFundHolding fdlsx_holdings[] =
{
   "MCD",
   "BKNG",
   "MAR",
   "HLT",
   "YUM",
   "CZR",
   "QSR",
   "SBUX",
   "DPZ",
   "LVS",
   "ABNB",
   "CHDN",
   "CMG",
   "GPN",
   "FWONA",
   "MTN",
   "LYV",
   "PDYPY",
   "EAT",
   "VAC",
   "ECL",
   "USFD",
   "BALY",
   "CTAS",
   "RUTH",
};


//Holdings of FGDFX (Fidelity Disruptors).
static const struct StMutualFundHolding fgdfx_holdings[] =
{
   "NVDA",
   "AMZN",
   "GOOG",
   "MSFT",
   "SE",
   "FB",
   "TMUS",
   "SBNY",
   "ISRG",
   "GOOGL",
   "NFLX",
   "ZS",
   "V",
   "MA",
   "ANET",
   "BLK",
   "TSM",
   "ACGL",
   "BRP",
   "ADBE",
   "COF",
   "ADYEY",
   "AMT",
   "TER",
};


//Holdings of FLOWX (Fidelity Water Sustainability)
static const struct StMutualFundHolding flowx_holdings[] =
{
   "ROP",
   "AWK",
   "TTEK",
   "ZWS",
   "HLMAF",
   "AQUA",
   "IEX",
   "TDY",
   "IR",
   "WMS",
   "DHR",
   "XYL",
   "OLN",
   "INDT",
   "AWR",
   "BMI",
   "PNR",
   "VEOEY",
   "MSEX",
   "WTS",
   "FI-N",
   "MEG",
   "STRNY",
   "MWA",
};


//Holdings of FMAGX (Fidelity Magellan).
static const struct StMutualFundHolding fmagx_holdings[] =
{
   "AAPL",
   "MSFT",
   "AMZN",
   "NVDA",
   "FB",
   "GOOGL",
   "GOOG",
   "UNH",
   "HD",
   "ADBE",
   "CRM",
   "NFLX",
   "COST",
   "AMD",
   "DHR",
   "TMO",
   "ACN",
   "INTU",
   "MA",
   "NKE",
   "NEE",
   "LIN",
   "LRCX",
   "TXN",
};


//Holdings of FOCPX (Fidelity OTC Portfolio).
static const struct StMutualFundHolding focpx_holdings[] =
{
   "MSFT",
   "AAPL",
   "GOOGL",
   "AMZN",
   "NVDA",
   "FB",
   "GOOG",
   "MRVL",
   "RLNIY",
   "NFLX",
   "TSLA",
   "LULU",
   "ADBE",
   "TWTR",
   "IT",
   "REGN",
   "ASML",
   "TCEHY",
   "CRM",
   "INTU",
   "SNAP",
   "VRSK",
   "HBAN",
   "ALNY",
   "LRCX",
};


//Holdings of FSCSX (Fidelity Select Software & IT Services Portfolio).
static const struct StMutualFundHolding fscsx_holdings[] =
{
   "MSFT",
   "ADBE",
   "CRM",
   "V",
   "MA",
   "PYPL",
   "GOOGL",
   "ADSK",
   "CTSH",
   "PANW",
   "WDAY",
   "ACN",
   "ORCL",
   "MDB",
   "INTU",
   "IBM",
   "CGEMY",
   "TWLO",
   "FB",
   "GPN",
   "HUBS",
   "AKAM",
   "ZEN",
   "ESTC",
};


//Holdings of FSDIX (Fidelity Strategic Dividend and Income).
static const struct StMutualFundHolding fsdix_holdings[] =
{
   "PG",
   "PLD",
   "KO",
   "CSCO",
   "CMCSA",
   "LLY",
   "ABBV",
   "BMY",
   "NEE",
   "CUBE",
   "PEP",
   "JNJ",
   "MCD",
   "BLK",
   "AMGN",
   "DLR",
   "MRK",
   "EQIX",
   "UPS",
   "VZ",
   "LAMR",
   "ESS",
   "MAA",
   "VTR",
};


//Holdings of FSELX (Fidelity Select Semiconductors).
static const struct StMutualFundHolding fselx_holdings[] =
{
   "NVDA",
   "NXPI",
   "MRVL",
   "MCHP",
   "ON",
   "ADI",
   "LRCX",
   "TER",
   "TXN",
   "AVGO",
   "GFS",
   "QCOM",
   "AMD",
   "CDNS",
   "TSM",
   "XLNX",
   "NVMI",
   "AMAT",
   "INTC",
   "CRUS",
   "MTSI",
   "KLAC",
   "JBL",
   "DIOD",
};


//Holdings of FSMEX (Fidelity Select Medical Technology & Devices Portfolio).
static const struct StMutualFundHolding fsmex_holdings[] =
{
   "TMO",
   "DHR",
   "BSX",
   "PODD",
   "DXCM",
   "CRL",
   "RMD",
   "PEN",
   "BIO",
   "ABT",
   "LZAGY",
   "ISRG",
   "AVTR",
   "SDMHF",
   "WST",
   "BRKR",
   "HUM",
   "HOLX",
   "MASI",
   "SYK",
   "TNDM",
   "NVST",
   "ALGN",
   "INSP",
   "TXG",
};


//Holdings of FSPCX (Fidelity Select Insurance Portfolio).
static const struct StMutualFundHolding fspcx_holdings[] =
{
   "MMC",
   "CB",
   "TRV",
   "AIG",
   "AJG",
   "HIG",
   "AON",
   "MET",
   "PGR",
   "ALL",
   "WLTW",
   "BRO",
   "RGA",
   "BRKB",
   "PRU",
   "PFG",
   "ACGL",
   "ARES",
   "APO",
   "PRI",
   "AGO",
   "L",
   "ATH",
   "FAF",
   "AIZ",
};


//Holdings of FSPHX (Fidelity Select Health Care Portfolio).
static const struct StMutualFundHolding fsphx_holdings[] =
{
   "UNH",
   "DHR",
   "HUM",
   "BSX",
   "LLY",
   "CI",
   "TMO",
   "PODD",
   "PEN",
   "CNC",
   "REGN",
   "ARGX",
   "ALNY",
   "BIO",
   "AZN",
   "RHHBY",
   "LZAGY",
   "HCA",
   "RPRX",
   "ASND",
   "BRKR",
   "TXG",
   "MASI",
   "ZNTL",
   "HOLX",
};


//Holdings of FSPTX (Fidelity Select Technology Portfolio).
static const struct StMutualFundHolding fsptx_holdings[] =
{
   "MSFT",
   "AAPL",
   "NVDA",
   "CRM",
   "ADBE",
   "MA",
   "V",
   "RUN",
   "JBL",
   "ON",
   "FLEX",
   "NXPI",
   "CLVT",
   "MCHP",
   "PYPL",
   "INTU",
   "WDAY",
   "MRVL",
   "MNTV",
   "HTHIY",
   "RLNIY",
   "GOOGL",
   "AMAT",
   "UBER",
   "TWLO",
};


//Holdings of FSRPX (Fidelity Select Retailing).
static const struct StMutualFundHolding fsrpx_holdings[] =
{
   "AMZN",
   "HD",
   "LOW",
   "TGT",
   "EBAY",
   "DG",
   "ROST",
   "BURL",
   "DLTR",
   "FTCH",
   "KSS",
   "BJ",
   "FIVE",
   "ULTA",
   "CPRI",
   "FND",
   "BBWI",
   "TJX",
   "JWN",
   "OLLI",
   "CVNA",
   "PVH",
   "TPR",
   "MELI",
   "AEO",
};


//Holdings of FSUTX (Fidelity Select Utilities Portfolio).
static const struct StMutualFundHolding fsutx_holdings[] =
{
   "NEE",
   "EXC",
   "SO",
   "SRE",
   "PCG",
   "EIX",
   "FE",
   "PEG",
   "AES",
   "D",
   "DUK",
   "AEP",
   "EVRG",
   "CNP",
   "NI",
   "ETR",
   "VST",
   "NEP",
   "NRG",
   "OGE",
   "NOVA",
   "CWEN",
};


static const struct StMutualFund mutual_funds[] =
{
   {
      "DOTD",
      "Dogs of the Dow",
      1.12,
      dotd_holdings,
      sizeof(dotd_holdings) / sizeof(dotd_holdings[0])
   },
   {
      "FAGIX",
      "Fidelity Capital and Income",
      1.0902,
      fagix_holdings,
      sizeof(fagix_holdings) / sizeof(fagix_holdings[0])
   },
   {
      "FARMX",
      "Fidelity Agricultural Productivity",
      1.12,
      farmx_holdings,
      sizeof(farmx_holdings) / sizeof(farmx_holdings[0])
   },
   {
      "FBIOX",
      "Fidelity Select Biotechnology",
      1.1347,
      fbiox_holdings,
      sizeof(fbiox_holdings) / sizeof(fbiox_holdings[0])
   },
   {
      "FBSOX",
      "Fidelity Select IT Services",
      1.2180,
      fbsox_holdings,
      sizeof(fbsox_holdings) / sizeof(fbsox_holdings[0])
   },
   {
      "FCNTX",
      "Fidelity Contrafund",
      1.2263,
      fcntx_holdings,
      sizeof(fcntx_holdings) / sizeof(fcntx_holdings[0])
   },
   {
      "FCPGX",
      "Fidelity Small Cap Growth",
      1.2041,
      fcpgx_holdings,
      sizeof(fcpgx_holdings) / sizeof(fcpgx_holdings[0])
   },
   {
      "FDLSX",
      "Fidelity Select Leisure",
      1.1730,
      fdlsx_holdings,
      sizeof(fdlsx_holdings) / sizeof(fdlsx_holdings[0])
   },
   {
      "FGDFX",
      "Fidelity Disruptors",
      1.15,
      fgdfx_holdings,
      sizeof(fgdfx_holdings) / sizeof(fgdfx_holdings[0])
   },
   {
      "FLOWX",
      "Fidelity Water Sustainability",
      1.12,
      flowx_holdings,
      sizeof(flowx_holdings) / sizeof(flowx_holdings[0])
   },
   {
      "FMAG",
      "Fidelity Magellan",
      1.2061,
      fmagx_holdings,
      sizeof(fmagx_holdings) / sizeof(fmagx_holdings[0])
   },
   {
      "FOCPX",
      "Fidelity OTC Portfolio",
      1.2794,
      focpx_holdings,
      sizeof(focpx_holdings) / sizeof(focpx_holdings[0])
   },
   {
      "FSCSX",
      "Fidelity Select Software & IT Services Portfolio",
      1.2804,
      fscsx_holdings,
      sizeof(fscsx_holdings) / sizeof(fscsx_holdings[0])
   },
   {
      "FSDIX",
      "Fidelity Strategic Dividend & Income",
      1.1168,
      fsdix_holdings,
      sizeof(fsdix_holdings) / sizeof(fsdix_holdings[0])
   },
   {
      "FSELX",
      "Fidelity Select Semiconductors",
      1.3506,
      fselx_holdings,
      sizeof(fselx_holdings) / sizeof(fselx_holdings[0])
   },
   {
      "FSMEX",
      "Fidelity Select Medical Technology & Devices Portfolio",
      1.2527,
      fsmex_holdings,
      sizeof(fsmex_holdings) / sizeof(fsmex_holdings[0])
   },
   {
      "FSPCX",
      "Fidelity Select Insurance Portfolio",
      1.1143,
      fspcx_holdings,
      sizeof(fspcx_holdings) / sizeof(fspcx_holdings[0])
   },
   {
      "FSPHX",
      "Fidelity Select Health Care Portfolio",
      1.1943,
      fsphx_holdings,
      sizeof(fsphx_holdings) / sizeof(fsphx_holdings[0])
   },
   {
      "FSPTX",
      "Fidelity Select Technology Portfolio",
      1.3275,
      fsptx_holdings,
      sizeof(fsptx_holdings) / sizeof(fsptx_holdings[0])
   },
   {
      "FSRPX",
      "Fidelity Select Retailing",
      1.2318,
      fsrpx_holdings,
      sizeof(fsrpx_holdings) / sizeof(fsrpx_holdings[0])
   },
   {
      "FSUTX",
      "Fidelity Select Utilities Portfolio",
      1.1321,
      fsutx_holdings,
      sizeof(fsutx_holdings) / sizeof(fsutx_holdings[0])
   },
};


static const char* stock_name_lookup(const char* symbol)
{
   size_t ui;

   for (ui = 0; ui < ((sizeof(holding_lookup))/(sizeof(holding_lookup[0]))); ui++)
   {
      if (!strcmp(symbol, holding_lookup[ui].symbol))
         return holding_lookup[ui].desc;
   }

   //If nothing found, NULL.
   return nullptr;
}


static size_t stock_name_index_lookup(const char* symbol)
{
   size_t ui;

   for (ui = 0; ui < ((sizeof(holding_lookup)) / (sizeof(holding_lookup[0]))); ui++)
   {
      if (!strcmp(symbol, holding_lookup[ui].symbol))
         return ui;
   }

   //If nothing found, 0.
   return 0;
}


static int comparison_function_largest_rounded_amount_first(const void* a_raw, const void* b_raw)
{
   const struct StatisticsTableRow* a, * b;

   a = (const struct StatisticsTableRow*)a_raw;
   b = (const struct StatisticsTableRow*)b_raw;

   if (a->popularity > b->popularity)
      return -1;
   else if (a->popularity < b->popularity)
      return 1;
   else
      return 0;
}


static int comparison_function_alphabetical_order_symbol(const void* a_raw, const void* b_raw)
{
   const struct StatisticsTableRow* a, * b;

   a = (const struct StatisticsTableRow*)a_raw;
   b = (const struct StatisticsTableRow*)b_raw;

   //printf("%llu %llu\n", (unsigned long long)a->index, (unsigned long long)b->index);

   //return 0;
   return(strcmp(holding_lookup[a->index].symbol, holding_lookup[b->index].symbol));
}


int main(int argc, char* argv[])
{
   struct StatisticsTableRow* st = nullptr;
   size_t index_limit;

   //Arguments are ignored.  If there are arguments, error.
   if (argc != 1)
   {
      printf("Arguments not allowed.\n");
      return 1;
   }

   //Check design rules.
   {
      size_t fund;
      size_t stock;

      for (fund = 0; fund < ((sizeof(mutual_funds))/(sizeof(mutual_funds[0]))); fund++)
      {
         for (stock = 0; stock < mutual_funds[fund].n_holdings; stock++)
         {
            if (!stock_name_lookup(mutual_funds[fund].holdings[stock].symbol))
            {
               printf("Stock symbol %s not found.  Fatal.\n", mutual_funds[fund].holdings[stock].symbol);
               return 1;
            }
         }
      }
   }

   //Allocate memory for the statistics table.  Fatal if can't allocate the memory.
   st = (struct StatisticsTableRow*)malloc(((sizeof(holding_lookup))/(sizeof(holding_lookup[0]))) * (sizeof(struct StatisticsTableRow)));
   if (!st)
   {
      printf("Fatal malloc() failure.\n");
      return 1;
   }

   //Zero out the statistics structure.  I don't know what the C standard says, but in practice a bitwise pattern
   //of 0's causes doubles to be 0.
   memset(st, 0, ((sizeof(holding_lookup)) / (sizeof(holding_lookup[0]))) * (sizeof(struct StatisticsTableRow)));

   //Set the row values of the statistics structure.
   {
      size_t row;

      for (row = 0; row < (sizeof(holding_lookup) / sizeof(holding_lookup[0])); row++)
      {
         st[row].index = row;
      }
   }

   //For each mutual fund, add in the popularities.
   {
      size_t fund;
      size_t stock;
      size_t index;
      double local_popularity, n_holdings, return_squared, position_weight;

      for (fund = 0; fund < ((sizeof(mutual_funds)) / (sizeof(mutual_funds[0]))); fund++)
      {
         if (diag_output)
            printf("Fund: %llu  Symbol: %-5s  Description: %s\n", (long long int)fund, mutual_funds[fund].symbol, mutual_funds[fund].desc);

         if (diag_output)
            printf("N_holdings: %llu\n", (long long int)mutual_funds[fund].n_holdings);

         return_squared = mutual_funds[fund].arr * mutual_funds[fund].arr;

         if (diag_output)
            printf("return_squared: %f\n", return_squared);

         n_holdings = (double)mutual_funds[fund].n_holdings;

         for (stock = 0; stock < mutual_funds[fund].n_holdings; stock++)
         {
            index = stock_name_index_lookup(mutual_funds[fund].holdings[stock].symbol);

            if (diag_output)
               printf("Stock: %llu  Symbol: %-5s  LUT Index: %llu\n", (long long int)stock, mutual_funds[fund].holdings[stock].symbol, (long long int)index);

            if (mutual_funds[fund].n_holdings <= 1)
            {
               position_weight = weight_max;
            }
            else
            {
               position_weight = weight_max - ((weight_max - weight_min) * (double)stock / ((double)mutual_funds[fund].n_holdings - 1));
            }

            if (diag_output)
               printf("position_weight:  %f\n", position_weight);

            local_popularity = return_squared * position_weight;

            if (diag_output)
               printf("Local popularity:  %f\n", local_popularity);

            st[index].popularity += local_popularity;
         }
      }
   }

   //Sort the table entries based on popularity.
   qsort(st, (sizeof(holding_lookup) / sizeof(holding_lookup[0])), sizeof(struct StatisticsTableRow), comparison_function_largest_rounded_amount_first);

   //Assign the dollar amounts.
   {
      double popularity_sum;
      size_t index_limit_dollar_amt = ((sizeof(holding_lookup)) / (sizeof(holding_lookup[0])));
      size_t holding;

      popularity_sum = 0.0;
      for (holding = 0; holding < ((sizeof(holding_lookup)) / (sizeof(holding_lookup[0]))); holding++)
      {
         popularity_sum += st[holding].popularity;
         if ((st[holding].popularity / popularity_sum) < (min_buy_amount / total_funds))
         {
            index_limit_dollar_amt = holding;
            break;
         }
      }

      index_limit = index_limit_dollar_amt;
      if (index_limit > max_n_stocks)
         index_limit = max_n_stocks;
      if (index_limit > ((sizeof(holding_lookup)) / (sizeof(holding_lookup[0]))))
         index_limit = ((sizeof(holding_lookup)) / (sizeof(holding_lookup[0])));

      popularity_sum = 0.0;
      for (holding = 0; holding < index_limit; holding++)
      {
         popularity_sum += st[holding].popularity;
      }

      for (holding = 0; holding < index_limit; holding++)
      {
         st[holding].funds_allocated = total_funds * st[holding].popularity / popularity_sum;
      }

      for (holding = 0; holding < index_limit; holding++)
      {
         double nrounding_units;

         nrounding_units = floor((st[holding].funds_allocated + round_increment/2.0)/round_increment);

         if (nrounding_units < 1.0)
            nrounding_units = 1.0;

         st[holding].funds_allocated_rounded = nrounding_units * round_increment;
      }
   }

   //Print the results.
   {
      size_t index;
      double total_ideal_amounts = 0.0;
      double total_rounded_amounts = 0.0;

      for (index = 0; index < index_limit; index++)
      {
         total_ideal_amounts += st[index].funds_allocated;
         total_rounded_amounts += st[index].funds_allocated_rounded;
      }

      printf("Total of ideal amounts   :  %10.2f\n", total_ideal_amounts);
      printf("Total of rounded amounts :  %10.2f\n", total_rounded_amounts);
      printf("\n");

      for (index = 0; index < index_limit; index++)
      {
         printf("%4llu %9.4f %10.2f %10.2f   %-6s %-59s\n", (long long unsigned)(index + 1), st[index].popularity, st[index].funds_allocated, st[index].funds_allocated_rounded, holding_lookup[st[index].index].symbol, holding_lookup[st[index].index].desc);
      }
   }

   //Sort the table entries we will use based on symbol, in alphabetical order.
   qsort(st, index_limit, sizeof(struct StatisticsTableRow), comparison_function_alphabetical_order_symbol);

   //Reprint the table in alphabetical order.
   {
      size_t index;

      printf("\n");

      for (index = 0; index < index_limit; index++)
      {
         printf("%4llu %9.4f %10.2f %10.2f   %-6s %-59s\n", (long long unsigned)(index + 1), st[index].popularity, st[index].funds_allocated, st[index].funds_allocated_rounded, holding_lookup[st[index].index].symbol, holding_lookup[st[index].index].desc);
      }
   }

   //Deallocate the memory for the statistics table.
   free(st);
   st = nullptr;

   //Happy path return.
   return 0;
}
