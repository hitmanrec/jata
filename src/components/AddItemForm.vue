<template>
	<form @submit.prevent>
		<h4>New item</h4>
		<simple-input-box 
			v-model="item.title"
			class="update-input title-input" 
			type="text" 
			placeholder="Title"
		/>
		
		<radio-row 
			class="update-input status-input"
			v-model:currentValue="item.status"
			:possibleValues="['planned', 'started', 'complete', 'dropped']"
		/>
		
		<simple-input-box 
			v-model="item.tags"
			class="update-input tags-input" 
			type="text" 
			placeholder="Tags"
		/>

		<textarea
			class="base-input update-input description-input"
			v-model="item.description"
			placeholder="Description"
			rows="4"
		>

		</textarea>
		
		<div class="update-input rating-input"  
			v-show="item.status === 'complete' || item.status === 'dropped'">
			<span class="rating-input__label">Rating</span>
			<colored-slider 
				class="rating-input__slider"
				v-model:currentValue="item.rating"
				:minValue="0"
				:maxValue="10"
				:editable="true"
			/>
		</div>
		
		<div class="update-input expectation-input"  
			v-show="item.status === 'started' || item.status === 'planned'">
			<span class="expectation-input__label">Expectation</span>
			<radio-row 
				v-model:currentValue="item.expectation"
				:possibleValues="['low', 'medium', 'high']"
			/>
		</div>
		
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
				status: 'planned',
				tags: '',
				rating: 0,
				expectation: 'low',
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

.status-input{
	width: 400px;
}

.title-input{
	width: 400px;
}

.update-button{
	align-self: flex-end;
	margin-top: 10px;
}

.expectation-input{
	display: flex;
	flex-direction: row;
	align-items: center;
	gap: 10px;
	width: 300px;

}

.rating-input{
	display: flex;
	flex-direction: row;
	align-items: center;
	gap: 10px;
	margin-bottom: 10px;
	width: 300px;
}

.rating-input__slider{
	flex: 1 1 0;
	width: 100%;
}
</style>