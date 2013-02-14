
(setq boost-root (getenv "PATH") )

(ede-cpp-root-project "SuperToll"
                :name "SuperToll"
                :file "~/src/ambrosys/tc-supertoll/trunk/src/Jamroot"
                :include-path '("/SuperToll"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "odeint"
                :name "odeint"
                :file "~/src/odeint-v2/Jamroot"
                :include-path '("/"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "GP"
                :name "GP"
                :file "~/src/genetic_programming/CMakeLists.txt"
                :include-path '("/"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))


(ede-cpp-root-project "SuperTollRedesign"
                :name "SuperTollRedesign"
                :file "~/src/ambrosys/tc-supertoll/branches/bd_new/Jamroot"
                :include-path '("/SuperToll"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))
