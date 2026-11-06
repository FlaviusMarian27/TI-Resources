import { StyleSheet, Text, View } from 'react-native';

export default function Despre() {
  return (
    <View style={styles.container}>
      <Text style={styles.titlu}>Activități personale</Text>
      <Text style={styles.text}>
        Aplicație demonstrativă pentru laboratorul 3 de la PAAM, care ilustrează
        navigarea cu Tabs și Stack în Expo Router.
      </Text>
      <Text style={styles.nume}>Marian Flavius-Andrei</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 16,
    gap: 12,
  },
  titlu: {
    fontSize: 24,
    fontWeight: 'bold',
  },
  text: {
    fontSize: 16,
    color: '#555',
  },
  nume: {
    fontSize: 16,
    fontWeight: '600',
  },
});