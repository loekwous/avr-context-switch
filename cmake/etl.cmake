include(FetchContent)

function(FetchETL)
    set(ETL_TAG "20.37.2")
    set(ETL_URL "https://github.com/ETLCPP/etl.git")

    FetchContent_Declare(etl
      GIT_REPOSITORY 
      ${ETL_URL}
      GIT_TAG
      ${ETL_TAG}
    )

    FetchContent_MakeAvailable(etl)

    add_compile_definitions(ETL_IS_USED)
endfunction()
