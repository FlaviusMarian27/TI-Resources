import { Alert, Button, Image, Platform, ScrollView, StyleSheet, Text, View } from 'react-native';
import ActivityCard from './components/ActivityCard';

export default function App() {
  const afiseazaDespre = () => {
    const mesaj = 'Aplicația ajută la organizarea activităților personale zilnice.';
    if(Platform.OS === 'web'){
      alert(mesaj);
    }else{
      Alert.alert('Despre aplicație', mesaj);
    }
  };

  return (
    <ScrollView contentContainerStyle={styles.container}>

      <View style={styles.header}>
        <Image source={require('./assets/icon.png')} style={styles.imagine}/>
        <Text style={styles.titlu}>Activități personale</Text>
        <Text style={styles.nume}>Marian Flavius-Andrei</Text>
      </View>

      <ActivityCard
        title='Alergare'
        description='30 de minute in parc'
        category='Sport'
      />

      <ActivityCard
        title='Citit'
        description='20 de pagini dintr-o carte de dezvoltare personală'
        category='Dezvoltare'
      />

      <ActivityCard
        title="Pregătirea proiectului final pentru materia Proiectarea și analiza aplicațiilor mobile"
        description="Finalizarea laboratorului 2"
        category="Facultate"
      />

      <Button title="Despre aplicatie" onPress={afiseazaDespre} />

    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    padding: 16,
    paddingTop: 48,
    paddingBottom: 32,
  },

  header:{
    alignItems: 'center',
    marginBottom: 24
  },

  imagine: {
    width: 120,
    height: 120,
    borderRadius: 24,
    marginBottom: 12,
  },

  titlu: {
    fontSize: 24,
    fontWeight: 'bold',
    textAlign: 'center',
  },

  nume: {
    fontSize: 16,
    color: '#555',
    marginTop: 4,
  },
});
