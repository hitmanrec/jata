<template>
	<form @submit.prevent>
		<h4>{{ actionType === 'add' ? 'New item' : 'Edit item' }}</h4>
		<simple-input-box 
			v-model="item_current.title"
			class="update-input title-input" 
			type="text" 
			placeholder="Title"
		/>
		
		<radio-row 
			class="update-input status-input"
			v-model:currentValue="item_current.status"
			:possibleValues="['planned', 'started', 'complete', 'dropped']"
			:colorPalette="['var(--planned-col)', 'var(--started-col)', 'var(--complete-col)', 'var(--dropped-col)']"
		/>
		
		<simple-input-box 
			v-model="item_current.tags"
			class="update-input tags-input" 
			type="text" 
			placeholder="Tags"
		/>

		<textarea
			class="base-input update-input description-input"
			v-model="item_current.description"
			placeholder="Description"
			rows="4"
		>

		</textarea>
		
		<div class="update-input rating-input"  
			v-show="item_current.status === 'complete' || item_current.status === 'dropped'">
			<span class="rating-input__label">Rating</span>
			<funny-rating 
				v-model:currentValue="item_current.rating"
			/>
		</div>
		
		<div class="update-input expectation-input"  
			v-show="item_current.status === 'started' || item_current.status === 'planned'">
			<span class="expectation-input__label">Expectation</span>
			<radio-row 
				v-model:currentValue="item_current.expectation"
				:possibleValues="['low', 'medium', 'high']"
				:colorPalette="['#888', '#dd8', '#8ff']"
			/>
		</div>
		
		<small-button 
			class="update-button" 
			@click="actionType === 'add' ? addItem() : updateItem()"
		>
			{{ actionType === 'add' ? 'Add' : 'Update' }}
		</small-button>
	</form>
</template>

<script>
export default {
	name: 'update-item-form',
	props: {
		item_to_update: {
			type: Object,
			default: () => ({})
		},
		actionType: {
			type: String,
			required: true
		}
	},
	data() {
		return {
			item_current: {
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
			this.item_current.creationTime = Date.now()
			this.item_current.completionTime = this.item_current.status === 'complete' || this.item_current.status === 'dropped' ? Date.now() : ''
			this.item_current.tags = this.item_current.tags ? this.item_current.tags.trim().split(',') : []
			this.$emit('add-item', this.item_current)
			this.resetForm()
		},
		updateItem() {
			this.item_current.status = this.item_current.status ? this.item_current.status : 'planned'
			this.item_current.expectation = this.item_current.expectation ? this.item_current.expectation : 'low'
			this.item_current.completionTime = this.item_current.status === 'complete' || this.item_current.status === 'dropped' ? Date.now() : ''
			this.item_current.tags = this.item_current.tags ? this.item_current.tags.trim().split(',') : []
			this.$emit('update-item', this.item_current)
			this.resetForm()
		},
		resetForm() {
			this.item_current = {
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
	watch: {
		item_to_update: {
			handler(newVal) {
				if (newVal && Object.keys(newVal).length > 0) {
					// Deep clone and convert tags array to string for editing
					this.item_current = {
						...newVal,
						tags: Array.isArray(newVal.tags) ? newVal.tags.join(', ') : (newVal.tags || '')
					}
				} else {
					this.resetForm()
				}
			},
			immediate: true,
			deep: true
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
	height: 20px;
	display: flex;
	flex-direction: row;
	align-items: center;
	gap: 10px;
	width: 300px;

}

.rating-input{
	height: 20px;
	display: flex;
	flex-direction: row;
	align-items: center;
	width: 300px;
	gap: 10px;
}

</style>