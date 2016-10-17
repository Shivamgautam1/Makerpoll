angular.module('makerPoll', [])
.controller('Result', function($scope, $http) {
    $http.get('http://data.sparkfun.com/output/roaE2LAoD2TlA6ElNpEg/latest.json?page=1').
        then(function(response) {
            $scope.poll = response.data;
        });
});