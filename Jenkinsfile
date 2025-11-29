pipeline {
   agent { label 'linux'}
   stages {
      stage('run container'){
         when { environment name: 'NODE_NAME', value :'server'}
         steps {
            sh 'echo node name is $NODE_NAME'
            sh './jenkins/run_container.sh'
         }
      }
      stage('test') {
         steps {
            sh './jenkins/ceedling_test.sh $NODE_NAME'
         }
      }
      stage('build') {
         steps {
            sh './jenkins/platformio_build.sh $NODE_NAME'
         }
      }
   }
   post {
      always {
         script{
            if (env.NODE_NAME == 'server') {
               sh './jenkins/stop_container.sh'
            }
         }
      }
      failure {
         mail bcc:'', body: "Jenkins reports a failed pipeline : #${BUILD_NUMBER} in ${BRANCH_NAME}", cc: '', from: 'jenkins', replyTo: '', subject: 'Pipeline failed', to: 'lulu@msikatana'
      }
   }
}
