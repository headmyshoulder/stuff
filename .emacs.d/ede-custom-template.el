
(setq boost-root (getenv "BOOST_ROOT") )

(ede-cpp-root-project "SuperToll"
                :name "SuperToll"
                :file "~/src/ambrosys/tc-supertoll/trunk/src/Jamroot"
                :include-path '("/SuperToll"  )
                :system-include-path '( boost-root )
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "odeint"
                :name "odeint"
                :file "~/src/odeint-v2/Jamroot"
                :include-path '("/"  )
                :system-include-path '( boost-root )
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "GP"
                :name "GP"
                :file "~/src/genetic_programming/CMakeLists.txt"
                :include-path '("/"  )
                :system-include-path '( boost-root )
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "GithubStuff"
                :name "GithubStuff"
                :file "~/src/github_stuff/README.md"
                :include-path '("/"  )
                :system-include-path '( boost-root )
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))
