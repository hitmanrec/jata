<template>
	<form @submit.prevent>
		<h4>Add Film</h4>
		<simple-input-box 
			v-model="item.title"
			class="update-input" 
			type="text" 
			placeholder="Title"
		/>
		
		<simple-input-box 
			v-model="item.status"
			class="update-input" 
			type="text" 
			placeholder="Status"
		/>
		
		<simple-input-box 
			v-model="item.tags"
			class="update-input" 
			type="text" 
			placeholder="Tags"
		/>
		
		<simple-input-box 
			v-model="item.rating"
			class="update-input" 
			type="text" 
			placeholder="Rating"
		/>
		
		<simple-input-box 
			v-model="item.expectation"
			class="update-input" 
			type="text" 
			placeholder="Expectation"
		/>
		
		<small-button 
			class="update-button" 
			@click="addItem"
		>
			Add
		</small-button>
	</form>
</template>

<script>
export default {
	data() {
		return {
			item: {
				title: '',
				status: '',
				tags: '',
				rating: '',
				expectation: '',
				description: '',
				completionTime: ''
			}
		}
	},
	methods: {
		addItem() {
			this.item.creationTime = Date.now()
			this.item.tags = this.item.tags.split(',')
			this.$emit('add-item', this.item)
			this.item = {
				title: '',
				status: '',
				tags: '',
				rating: '',
				expectation: '',
				description: '',
				completionTime: ''
			}
		}
	}
}
</script>

<style scoped>

form{
	display: flex;
	flex-direction: column;
}

.update-input{
	width: 100%;
	margin-top: 10px;

}

.update-button{
	align-self: flex-end;
	margin-top: 10px;
}

</style>